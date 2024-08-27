#include <iostream>
#include <fstream>
#include <sstream>
#include "flux_interp.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline2d.h>
#include <gsl/gsl_interp.h>


bool readTokamakData(const std::string &filename, TokamakData &data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return false;
    }

    // Read the first line to ge nw, nh
    std::string line;
    std::getline(file, line);

    // get nw, nh (from the last 12 characters of the first line)
    std::istringstream first_line(line.substr(line.length() - 12));
    int idum; // Unused variable
    first_line >> idum >> data.nw >> data.nh;

    data.psirz = new double[data.nw * data.nh];

    // Read geometric parameters
    file >> data.rdim >> data.zdim >> idum >> data.rleft >> data.zmid;

    // Read sibry
    double rmaxis, zmaxis, simag, bcentr;
    file >> rmaxis >> zmaxis >> simag >> data.sibry >> bcentr;

    // Skip unnecessary parameters
    double current, xdum;
    file >> current >> simag >> xdum >> rmaxis >> xdum;
    file >> zmaxis >> xdum >> data.sibry >> xdum >> xdum;

    // Skip fpol, pres, ffprim, pprime arrays
    int skipCount = data.nw;  // Number of data points to skip
    int charsPerData = 16;    // Number of characters per data point

    file.ignore(4 * skipCount * charsPerData);

    // Read the psirz into 1D array
    for (int j = 0; j < data.nh; j++) {
        for (int i = 0; i < data.nw; i++) {
            file >> data.psirz[j * data.nw + i];
        }
    }

    file.close();
    return true;
}

void initSplineInterpolation(const TokamakData &data, SplineInterpolation &interp) {
    interp.spline = gsl_spline2d_alloc(gsl_interp2d_bicubic, data.nw, data.nh);

    interp.racc = gsl_interp_accel_alloc();
    interp.zacc = gsl_interp_accel_alloc();

    interp.r = new double[data.nw];
    interp.z = new double[data.nh];

    interp.pflux = data.psirz;

    double dr = data.rdim / (data.nw - 1);
    double dz = data.zdim / (data.nh - 1);

    // Set up r and z arrays 
    for (int i = 0; i < data.nw; i++) {
        interp.r[i] = data.rleft + i * dr;
    }
    for (int j = 0; j < data.nh; j++) {
        interp.z[j] = data.zmid - 0.5 * data.zdim + j * dz;
    }

    gsl_spline2d_init(interp.spline, interp.r, interp.z, interp.pflux, data.nw, data.nh);
}

void evaluatePoloidalFlux(const double& r, const double& z, const SplineInterpolation &interp, const TokamakData &data, double &Pflux) {

    Pflux = gsl_spline2d_eval(interp.spline, r, z, interp.racc, interp.zacc);
    
    if (Pflux > data.sibry || z < -3.4) {
        std::cerr << "Warning: Out of plasma boundary. Exiting function." << std::endl;
        exit(EXIT_FAILURE);
    }
}

double evaluatePoloidalFlux(double r, double z, const SplineInterpolation &interp, const TokamakData &data) {
    double Pflux = gsl_spline2d_eval(interp.spline, r, z, interp.racc, interp.zacc);

    if (Pflux > data.sibry || z < -3.4) {
        std::cerr << "Warning: Out of plasma boundary. Exiting function." << std::endl;
        exit(EXIT_FAILURE);
    }

    return Pflux;
}

void freeSplineInterpolation(SplineInterpolation &interp) {
    gsl_spline2d_free(interp.spline);
    gsl_interp_accel_free(interp.racc);
    gsl_interp_accel_free(interp.zacc);

    delete[] interp.r;
    delete[] interp.z;
    // Note: interp.pflux points to data.psirz, so we do not delete it here
}

void freeTokamakData(TokamakData &data) {
    delete[] data.psirz;
}