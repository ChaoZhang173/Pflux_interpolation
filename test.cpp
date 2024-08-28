#include "flux_interp.h"
#include <iostream>

int main() {

    std::string filename = "G_DTHMODE24_ITER_MR_00400.TXT";
    FluxInterpolator* interpolator = new FluxInterpolator(filename);

    double r = 6.25742;
    double z = 1.77172;
    // should return -11.0815

    interpolator->printData();

    double flux = interpolator->evaluatePoloidalFlux(r, z);
    std::cout << "Poloidal flux at (" << r << ", " << z << ") is: " << flux << std::endl;

    double Pflux;
    interpolator->evaluatePoloidalFlux(r, z, Pflux);
    std::cout << "Poloidal flux at (" << r << ", " << z << ") is: " << Pflux << std::endl;

    // test out of plasma boundary
    double r1 = 3.0;
    double z1 = 5.0;
    interpolator->evaluatePoloidalFlux(r1, z1, Pflux);
    std::cout << "Poloidal flux at (" << r1 << ", " << z1 << ") is: " << Pflux << std::endl;

    r1 = 6.2;
    z1 = -5.0;
    interpolator->evaluatePoloidalFlux(r1, z1, Pflux);
    std::cout << "Poloidal flux at (" << r1 << ", " << z1 << ") is: " << Pflux << std::endl;

    r1 = 8.0;
    z1 = 3.5;
    interpolator->evaluatePoloidalFlux(r1, z1, Pflux);
    std::cout << "Poloidal flux at (" << r1 << ", " << z1 << ") is: " << Pflux << std::endl;

    // test out of the grid boundary
    r1 = 10.0;
    z1 = 0.1;
    interpolator->evaluatePoloidalFlux(r1, z1, Pflux);
    std::cout << "Poloidal flux at (" << r1 << ", " << z1 << ") is: " << Pflux << std::endl;

    r1 = 5.5;
    z1 = 10.0;
    interpolator->evaluatePoloidalFlux(r1, z1, Pflux);
    std::cout << "Poloidal flux at (" << r1 << ", " << z1 << ") is: " << Pflux << std::endl;

    return 0;
}
