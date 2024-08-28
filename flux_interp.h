#ifndef FLUX_INTERP_H
#define FLUX_INTERP_H

#include <gsl/gsl_spline2d.h>
#include <string>

class FluxInterpolator {

public:
    /**
    * Constructor that initializes the interpolator with data from the specified file.
    *
    * @param filename: The name of the file containing Plasma data.
    */
    FluxInterpolator(const std::string &filename);

    // Destructor to free allocated memory
    ~FluxInterpolator();
    
    /**
    * Evaluates the poloidal flux at a given (r, z) position using spline interpolation.
    * The flux value is returned via a reference parameter.
    *
    * @param r: Reference to the radial position where the flux is evaluated.
    * @param z: Reference to the vertical position where the flux is evaluated.
    * @param Pflux: Reference to a double where the evaluated flux value will be stored.
    */
    void evaluatePoloidalFlux(const double& r, const double& z, double &Pflux);

    /**
    * Evaluates the poloidal flux at a given (r, z) position using spline interpolation.
    * This version returns the flux value directly.
    *
    * @param r: The radial position where the flux is evaluated.
    * @param z: The vertical position where the flux is evaluated.
    * @return: Returns the evaluated flux value as a double.
    */
    double evaluatePoloidalFlux(double r, double z);

    // Prints the Plasma data to the console
    void printData();

private:
    struct PlasmaData {
        int nw, nh;        // Number of grid points in the radial (nw) and vertical (nh) directions
        double rdim;       // Radial length of computational domain (meter)
        double zdim;       // Vertical length of computational domain (meter)
        double rleft;      // Left boundary in the radial direction
        double zmid;       // Z center of the computational domain
        double sibry;      // Plasma boundary value of the poloidal flux (Weber/rad)
        double* psirz;     // 1D array (column oriented) of poloidal flux on the grid (Webner/rad) 
    };

    // GSL interpolation objects
    struct SplineInterp {
        gsl_spline2d *spline;  // GSL spline object for spline interpolation
        gsl_interp_accel *racc; // GSL interpolation accelerator object for r-direction
        gsl_interp_accel *zacc; // GSL interpolation accelerator object for z-direction
        double* r;              // Array of r-coordinates (radial grid points)
        double* z;              // Array of z-coordinates (vertical grid points)
        double* pflux;          // poloidal flux on the grid (Webner/rad), shared with psirz
    };
    
    PlasmaData* data;
    SplineInterp* interp;

    // Reads Plasma data from a specified file.
    bool readPlasmaData(const std::string &filename);

    // Initializes the GSL spline interpolation based on the provided Plasma data.
    void initSplineInterpolation();

    // Frees the memory allocated for the GSL spline interpolation.
    void freeSplineInterpolation();

    // Frees the memory allocated for the Plasma data.
    void freePlasmaData();
};

#endif // FLUX_INTERP_H
