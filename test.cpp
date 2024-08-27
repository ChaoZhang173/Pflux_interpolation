#include <iostream>
#include "flux_interp.h"

int main() {

    TokamakData* data = new TokamakData;
    SplineInterpolation* interp = new SplineInterpolation;

    // Read data from the file 
    if (!readTokamakData("G_DTHMODE24_ITER_MR_00400.TXT", *data)) {
        std::cerr << "Failed to read Tokamak data from the file." << std::endl;
        delete data;
        delete interp;
        return 1;
    }

    // Initialize spline interpolation
    initSplineInterpolation(*data, *interp);

    std::cout<<"rleft = "<<data->rleft<<"; rdim = "<<data->rdim<<"; zmid = "<<data->zmid<<"; zdim = "<<data->zdim<<"; sibry = "<<data->sibry<<std::endl;
    std::cout<<"r range: "<<interp->r[0]<<"; ~ "<<interp->r[data->nw-1]<<std::endl;
    std::cout<<"z range: "<<interp->z[0]<<"; ~ "<<interp->z[data->nh-1]<<std::endl;
    
    // Test point (r, z) for poloidal flux calculation
    double r = 5.8; 
    double z = 1.5; 
    double Pflux;  

    // Get the poloidal flux value at the given (r, z)
    evaluatePoloidalFlux(r, z, *interp, *data, Pflux);
    std::cout << "Poloidal flux at (" << r << ", " << z << ") is: " << Pflux << " Wb/rad" << std::endl;

    // Clean up
    freeSplineInterpolation(*interp);
    freeTokamakData(*data);
    delete data;
    delete interp;

    return 0;
}
