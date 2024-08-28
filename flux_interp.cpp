#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <gsl/gsl_errno.h>
#include "flux_interp.h"

FluxInterpolator::FluxInterpolator(const std::string &filename) {
    data = new PlasmaData;
    interp = new SplineInterp;

    data->psirz = nullptr;
    interp->spline = nullptr;
    interp->racc = nullptr;
    interp->zacc = nullptr;
    interp->r = nullptr;
    interp->z = nullptr;
    interp->pflux = nullptr;

    if (!readPlasmaData(filename)) {
        std::cerr << "Error: Failed to read Plasma data from " << filename << std::endl;
        exit(EXIT_FAILURE); 
    }

    initSplineInterpolation();
}

FluxInterpolator::~FluxInterpolator() {
    freeSplineInterpolation();
    freePlasmaData();
    delete data;
    delete interp;
}

bool FluxInterpolator::readPlasmaData(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    // Read the first line to ge nw, nh
    std::string line;
    std::getline(file, line);

    // get nw, nh (from the last 12 characters of the first line)
    std::istringstream first_line(line.substr(line.length() - 12));
    int idum; // Unused variable
    first_line >> idum >> data->nw >> data->nh;

    // Read the second line: rdim, zdim, rcentr, rleft, zmid
    double rcentr;
    file >> data->rdim >> data->zdim >> rcentr >> data->rleft >> data->zmid;

    // Read the third line: rmaxis, zmaxis, simag, sibry, bcentr
    double rmaxis, zmaxis, simag, bcentr;
    file >> rmaxis >> zmaxis >> simag >> data->sibry >> bcentr;

    // // for debugging
    // const int bufferSize = 16;
    // char buffer[bufferSize];
    // char current_buffer[bufferSize];
    // std::streampos current_pos;
    // // Display current file position
    // std::cout << "Current file position: " << file.tellg() << std::endl;
    // // Move back 16 bytes
    // current_pos = file.tellg();
    // file.seekg(current_pos - 16, std::ios::beg);
    // file.read(buffer, 16);
    // std::cout << "Previous 16 bytes: ";
    // for (int i = 0; i < 16; i++) {
    //     if (buffer[i] == '\n') {
    //         std::cout << "[\\n]";
    //     } else if (buffer[i] == '\r') {
    //         std::cout << "[\\r]";
    //     } else if (buffer[i] == ' ') {
    //         std::cout << "[space]";
    //     } else {
    //         std::cout << buffer[i];
    //     }
    // }
    // std::cout << std::endl;
    // // Move to the current position
    // file.seekg(current_pos, std::ios::beg);
    // // Read the next 16 bytes
    // file.read(current_buffer, 16);
    // std::cout << "Current and next 16 bytes: ";
    // for (int i = 0; i < 16; ++i) {
    //     if (current_buffer[i] == '\n') {
    //         std::cout << "[\\n]";
    //     } else if (current_buffer[i] == '\r') {
    //         std::cout << "[\\r]";
    //     } else if (current_buffer[i] == ' ') {
    //         std::cout << "[space]";
    //     } else {
    //         std::cout << current_buffer[i];
    //     }
    // }
    // std::cout << std::endl;
    // // Move to the current position
    // file.seekg(current_pos, std::ios::beg);

    // Skip current,simag,xdum,rmaxis,xdum,zmaxis,xdum,sibry,xdum,xdum
    // and fpol, pres, ffprim, pprime arrays
    int numCount = 10; // Number of values to skip
    int arrayCount = data->nw;  // array of data points to skip
    int charsPerData = 16;    // Number of characters per data point
    
    // // skip current,simag,xdum,rmaxis,xdum
    // file.ignore(5*charsPerData+2);

    // // Display current file position
    // std::cout << "Current file position: " << file.tellg() << std::endl;
    // // Move back 16 bytes
    // current_pos = file.tellg();
    // file.seekg(current_pos - 16, std::ios::beg);
    // file.read(buffer, 16);
    // std::cout << "Previous 16 bytes: ";
    // for (int i = 0; i < 16; i++) {
    //     if (buffer[i] == '\n') {
    //         std::cout << "[\\n]";
    //     } else if (buffer[i] == '\r') {
    //         std::cout << "[\\r]";
    //     } else if (buffer[i] == ' ') {
    //         std::cout << "[space]";
    //     } else {
    //         std::cout << buffer[i];
    //     }
    // }
    // std::cout << std::endl;
    // // Move to the current position
    // file.seekg(current_pos, std::ios::beg);
    // // Read the next 16 bytes
    // file.read(current_buffer, 16);
    // std::cout << "Current and next 16 bytes: ";
    // for (int i = 0; i < 16; ++i) {
    //     if (current_buffer[i] == '\n') {
    //         std::cout << "[\\n]";
    //     } else if (current_buffer[i] == '\r') {
    //         std::cout << "[\\r]";
    //     } else if (current_buffer[i] == ' ') {
    //         std::cout << "[space]";
    //     } else {
    //         std::cout << current_buffer[i];
    //     }
    // }
    // std::cout << std::endl;
    // // Move to the current position
    // file.seekg(current_pos, std::ios::beg);

    // // skip zmaxis,xdum,sibry,xdum,xdum
    // file.ignore(5*charsPerData+1);

    // // Display current file position
    // std::cout << "Current file position: " << file.tellg() << std::endl;
    // // Move back 16 bytes
    // current_pos = file.tellg();
    // file.seekg(current_pos - 16, std::ios::beg);
    // file.read(buffer, 16);
    // std::cout << "Previous 16 bytes: ";
    // for (int i = 0; i < 16; i++) {
    //     if (buffer[i] == '\n') {
    //         std::cout << "[\\n]";
    //     } else if (buffer[i] == '\r') {
    //         std::cout << "[\\r]";
    //     } else if (buffer[i] == ' ') {
    //         std::cout << "[space]";
    //     } else {
    //         std::cout << buffer[i];
    //     }
    // }
    // std::cout << std::endl;
    // // Move to the current position
    // file.seekg(current_pos, std::ios::beg);
    // // Read the next 16 bytes
    // file.read(current_buffer, 16);
    // std::cout << "Current and next 16 bytes: ";
    // for (int i = 0; i < 16; ++i) {
    //     if (current_buffer[i] == '\n') {
    //         std::cout << "[\\n]";
    //     } else if (current_buffer[i] == '\r') {
    //         std::cout << "[\\r]";
    //     } else if (current_buffer[i] == ' ') {
    //         std::cout << "[space]";
    //     } else {
    //         std::cout << current_buffer[i];
    //     }
    // }
    // std::cout << std::endl;
    // // Move to the current position
    // file.seekg(current_pos, std::ios::beg);

    int buff = static_cast<int>(std::ceil(data->nw/5.0));
    std::cout<<"buff = "<<buff<<std::endl;
    file.ignore(4 *(arrayCount * charsPerData + buff) + numCount * charsPerData + 3);
    
    // // Display current file position
    // std::cout << "Current file position: " << file.tellg() << std::endl;
    // // Move back 16 bytes
    // current_pos = file.tellg();
    // file.seekg(current_pos - 16, std::ios::beg);
    // file.read(buffer, 16);
    // std::cout << "Previous 16 bytes: ";
    // for (int i = 0; i < 16; i++) {
    //     if (buffer[i] == '\n') {
    //         std::cout << "[\\n]";
    //     } else if (buffer[i] == '\r') {
    //         std::cout << "[\\r]";
    //     } else if (buffer[i] == ' ') {
    //         std::cout << "[space]";
    //     } else {
    //         std::cout << buffer[i];
    //     }
    // }
    // std::cout << std::endl;
    // // Move to the current position
    // file.seekg(current_pos, std::ios::beg);
    // // Read the next 16 bytes
    // file.read(current_buffer, 16);
    // std::cout << "Current and next 16 bytes: ";
    // for (int i = 0; i < 16; ++i) {
    //     if (current_buffer[i] == '\n') {
    //         std::cout << "[\\n]";
    //     } else if (current_buffer[i] == '\r') {
    //         std::cout << "[\\r]";
    //     } else if (current_buffer[i] == ' ') {
    //         std::cout << "[space]";
    //     } else {
    //         std::cout << current_buffer[i];
    //     }
    // }
    // std::cout << std::endl;
    // file.seekg(current_pos, std::ios::beg);

    // Read the psirz matrix, which is stored column-wise
    data->psirz = new double[data->nw * data->nh];
    for (int j = 0; j < data->nh; ++j) {
        for (int i = 0; i < data->nw; ++i) {
            file >> data->psirz[j * data->nw + i];
        }
    }

    file.close();
    return true;
}

void FluxInterpolator::initSplineInterpolation() {
    interp->spline = gsl_spline2d_alloc(gsl_interp2d_bicubic, data->nw, data->nh);

    interp->racc = gsl_interp_accel_alloc();
    interp->zacc = gsl_interp_accel_alloc();

    interp->r = new double[data->nw];
    interp->z = new double[data->nh];
    interp->pflux = data->psirz;

    double dr = data->rdim / (data->nw - 1);
    double dz = data->zdim / (data->nh - 1);

    for (int i = 0; i < data->nw; ++i) {
        interp->r[i] = data->rleft + i * dr;
    }
    for (int j = 0; j < data->nh; ++j) {
        interp->z[j] = data->zmid - data->zdim / 2 + j * dz;
    }

    gsl_spline2d_init(interp->spline, interp->r, interp->z, interp->pflux, data->nw, data->nh);
}

void FluxInterpolator::evaluatePoloidalFlux(const double& r, const double& z, double &Pflux) {
    if (r < interp->r[0] || r > interp->r[data->nw - 1] || 
        z < interp->z[0] || z > interp->z[data->nh - 1]) {
        std::cerr << "Error: r or z value out of interpolation range. Exiting function." << std::endl;
        exit(EXIT_FAILURE);
    }
    Pflux = gsl_spline2d_eval(interp->spline, r, z, interp->racc, interp->zacc);
    
    if (Pflux > data->sibry || z < -3.4) {
        std::cerr << "Warning: Out of plasma boundary." << std::endl;
    }
}

double FluxInterpolator::evaluatePoloidalFlux(double r, double z) {

    double Pflux = gsl_spline2d_eval(interp->spline, r, z, interp->racc, interp->zacc);

    if (Pflux > data->sibry || z < -3.4) {
        std::cerr << "Warning: Out of plasma boundary." << std::endl;
        exit(EXIT_FAILURE);
    }

    return Pflux;
}

void FluxInterpolator::printData(){
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);

    std::cout << "nw = " << data->nw << "; nh = " << data->nh << std::endl;
    std::cout << "rdim = " << data->rdim << "; zdim = " << data->zdim << std::endl;
    std::cout << "rleft = " << data->rleft << "; zmid = " << data->zmid << std::endl;
    std::cout << "sibry = " << data->sibry << std::endl;
    std::cout<<"r range: "<<interp->r[0]<<"; ~ "<<interp->r[data->nw-1]<<std::endl;
    std::cout<<"z range: "<<interp->z[0]<<"; ~ "<<interp->z[data->nh-1]<<std::endl;
    // print first 5 data of psirz
    std::cout << std::scientific << std::setprecision(9);
    std::cout<<"psirz: (0:4,1): "<<std::endl;
    for(int i = 0; i < 5; i++){
        std::cout<<std::setw(16)<<data->psirz[i]<<" ";
    }
    std::cout<<std::endl;
    //print the last 5 data of psirz
    std::cout<<"psirz: (nw-5:nw-1,nh): "<<std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout<<std::setw(16)<<data->psirz[data->nw * data->nh - 5 + i]<<" ";
    }    
    std::cout<<std::endl;
    std::cout.copyfmt(oldState);
}

void FluxInterpolator::freeSplineInterpolation() {
    if (interp->spline) gsl_spline2d_free(interp->spline);
    if (interp->racc) gsl_interp_accel_free(interp->racc);
    if (interp->zacc) gsl_interp_accel_free(interp->zacc);
    delete[] interp->r;
    delete[] interp->z;
}

void FluxInterpolator::freePlasmaData() {
    delete[] data->psirz;
}
