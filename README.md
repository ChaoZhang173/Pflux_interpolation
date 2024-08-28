# Poloidal Flux Interpolator

## Overview

The `Poloidal Flux Interpolator` is a C++ implementation that uses bicubic spline interpolation (via the GNU Scientific Library - GSL) to evaluate the poloidal flux at specific radial (`r`) and vertical (`z`) coordinates within a Tokamak plasma domain. The flux data is read from a text file that contains plasma-related data structured according to the Fortran code conventions used in magnetic confinement fusion research.

## Features

- **Reading Plasma Data**: The program reads the plasma data from a text file and stores relevant grid and flux information for interpolation.
- **Bicubic Spline Interpolation**: Utilizes GSL's bicubic spline interpolation to evaluate the poloidal flux at any given point within the interpolation grid.
- **Safety Checks**: Includes safety checks to ensure the provided `r` and `z` values fall within the interpolation grid and within the plasma boundary.

## Requirements

- GNU Scientific Library (GSL) version 2.7.1 or above.
- C++11 standard or higher.
- a G EQDSK file using ITER standard. 

## Installation

1. **Install GSL** (if not already installed):
    ```bash
    sudo apt-get install libgsl-dev
    ```

2. **Clone the repository**:
    ```bash
    git clone https://github.com/yourusername/poloidal-flux-interpolator.git
    cd poloidal-flux-interpolator
    ```

3. **Compile the code**:
    ```bash
    g++ -std=c++11 flux_interp.cpp test.cpp -o poloidal_flux_interpolator $(gsl-config --cflags --libs)
    ```

## Usage

### Example Usage in `test.cpp`

The `test.cpp` file demonstrates how to use the `FluxInterpolator` class:

```cpp
#include "flux_interp.h"
#include <iostream>

int main() {

    std::string filename = "G_DTHMODE24_ITER_MR_00400.TXT";
    FluxInterpolator* interpolator = new FluxInterpolator(filename);

    double r = 6.25742;
    double z = 1.77172;

    interpolator->printData();

    double flux = interpolator->evaluatePoloidalFlux(r, z);
    std::cout << "Poloidal flux at (" << r << ", " << z << ") is: " << flux << std::endl;

    double Pflux;
    interpolator->evaluatePoloidalFlux(r, z, Pflux);
    std::cout << "Poloidal flux at (" << r << ", " << z << ") is: " << Pflux << std::endl;

    return 0;
}
``````

### Running the Program

After compiling, run the program using the following command:

```bash
./poloidal_flux_interpolator

The program will read the plasma data from the specified file, perform the interpolation, and output the interpolated flux values at specified coordinates.

### Important Notes

1. **Data Format**: The input text file containing the plasma data must be structured according to the Fortran format conventions expected by the program. This includes correct spacing and alignment of numerical data.
2. **Safety Checks**: The program includes checks to ensure that the provided `r` and `z` values are within the bounds of the interpolation grid. Additionally, it checks if the computed flux value exceeds the plasma boundary (`sibry`) or if `z` is below a certain threshold (`-3.4`). If these conditions are violated, the program will output a warning and terminate.
3. **Interpolation Limits**: The program assumes that `r` and `z` values fall within the ranges defined by the grid. If they do not, the interpolation will not be valid, and the program will exit with an error.
