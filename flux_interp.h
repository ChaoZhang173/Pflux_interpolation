#ifndef FLUX_INTERP_H
#define FLUX_INTERP_H

#include <gsl/gsl_spline2d.h>
#include <string>

struct TokamakData {
    int nw, nh;        // Number of grid points in the radial (nw) and vertical (nh) directions
    double rdim;       // Radial length of computational domain (meter)
    double zdim;       // Vertical length of computational domain (meter)
    double rleft;      // Left boundary in the radial direction
    double zmid;       // Z center of the computational domain
    double sibry;      // Plasma boundary value of the poloidal flux (Weber/rad)
    double* psirz;     // 1D array (column oriented) of poloidal flux on the grid (Webner/rad) 
};

// GSL interpolation objects
struct SplineInterpolation {
    gsl_spline2d *spline;  // GSL spline object for spline interpolation
    gsl_interp_accel *racc; // GSL interpolation accelerator object for r-direction
    gsl_interp_accel *zacc; // GSL interpolation accelerator object for z-direction
    double* r;              // Array of r-coordinates (radial grid points)
    double* z;              // Array of z-coordinates (vertical grid points)
    double* pflux;          // poloidal flux on the grid (Webner/rad), shared with psirz
};

/**
 * Reads Tokamak data from a specified file.
 *
 * @param filename: The name of the file containing Tokamak data.
 * @param data: Reference to a TokamakData struct to store the read data.
 * @return: Returns true if the data was successfully read, false otherwise.
 */
bool readTokamakData(const std::string &filename, TokamakData &data);

/**
 * Initializes the GSL spline interpolation based on the provided Tokamak data.
 *
 * @param data: Reference to the TokamakData containing grid and poloidal flux data.
 * @param interp: Reference to a SplineInterpolation struct to be initialized.
 */
void initSplineInterpolation(const TokamakData &data, SplineInterpolation &interp);

/**
 * Evaluates the poloidal flux at a given (r, z) position using spline interpolation.
 * The flux value is returned via a reference parameter.
 *
 * @param r: Reference to the radial position where the flux is evaluated.
 * @param z: Reference to the vertical position where the flux is evaluated.
 * @param interp: Reference to the SplineInterpolation object containing interpolation data.
 * @param data: Reference to the TokamakData object containing boundary value for safety checks.
 * @param Pflux: Reference to a double where the evaluated flux value will be stored.
 */
void evaluatePoloidalFlux(const double& r, const double& z, const SplineInterpolation &interp, const TokamakData &data, double &Pflux);

/**
 * Evaluates the poloidal flux at a given (r, z) position using spline interpolation.
 * This version returns the flux value directly.
 *
 * @param r: The radial position where the flux is evaluated.
 * @param z: The vertical position where the flux is evaluated.
 * @param interp: Reference to the SplineInterpolation object containing interpolation data.
 * @param data: Reference to the TokamakData object containing boundary value for safety checks.
 * @return: Returns the evaluated flux value as a double.
 */
double evaluatePoloidalFlux(double r, double z, const SplineInterpolation &interp, const TokamakData &data);

/**
 * Frees the memory allocated for the GSL spline interpolation.
 *
 * @param interp: Reference to the SplineInterpolation struct to be freed.
 */
void freeSplineInterpolation(SplineInterpolation &interp);

/**
 * Frees the memory allocated for the Tokamak data.
 *
 * @param data: Reference to the TokamakData struct to be freed.
 */
void freeTokamakData(TokamakData &data);

#endif // FLUX_INTERP_H
