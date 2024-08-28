# Poloidal Flux Interpolator

## Overview

The `Poloidal Flux Interpolator` is a C++ implementation that uses bicubic spline interpolation (via the GNU Scientific Library - GSL) to evaluate the poloidal flux at specific radial (`r`) and vertical (`z`) coordinates within a Tokamak plasma domain. The flux data is read from a G_EQDSK text file that contains plasma-related data structured according to the Fortran code conventions used in magnetic confinement fusion research.

## Features

- **Reading Plasma Data**: The program reads the plasma data from a text file and stores relevant grid and flux information for interpolation.
- **Bicubic Spline Interpolation**: Utilizes GSL's bicubic spline interpolation to evaluate the poloidal flux at any given point within the interpolation grid.
- **Safety Checks**: Includes safety checks to ensure the provided `r` and `z` values fall within the interpolation grid and within the plasma boundary.

## Requirements

- GNU Scientific Library (GSL) version 2.7.1 or above. Some older version may also work.
- C++11 standard or higher.
- a G EQDSK file using ITER standard. 

## Installation

1. **Install GSL** (if not already installed):
    ```bash
    sudo apt-get install libgsl-dev
    ```

2. **Clone the repository**:
    ```bash
    git clone https://github.com/ChaoZhang173/Pflux_interpolation.git
    cd Pflux_interpolation
    ```
3. **Include the header file**:
    ```cpp
    #include "flux_interp.h"

4. **Create the object and call the function**:
    ```cpp
    FluxInterpolator* interpolator = new FluxInterpolator(filename);
    interpolator->evaluatePoloidalFlux(r, z, Pflux);
    // or could use this function: 
    double flux = interpolator->evaluatePoloidalFlux(r, z);
    ```

5. **Compile the code**:
    ```bash
    g++ -std=c++11 flux_interp.cpp <your_code_file> -o <your_executable> $(gsl-config --cflags --libs)
    ```

## Usage

### Example Usage in `test.cpp`

The `test.cpp` file demonstrates how to use the `FluxInterpolator` class. 

### Important Notes

1. **Data Format**: The input text file containing the plasma data must be structured according to the ITER-standard G EDQSK format conventions expected by the program. This includes correct spacing and alignment of numerical data.
2. **Safety Checks**: The program includes checks to ensure that the provided `r` and `z` values are within the bounds of the interpolation grid. Additionally, it checks if the computed flux value exceeds the plasma boundary (`sibry`) or if `z` is below a certain threshold (`-3.4`), which is based on the geometry properity of tokamak plasma. If these conditions are violated, the program will output a warning (and may be terminated).
3. **Interpolation Limits**: The program assumes that `r` and `z` values fall within the ranges defined by the grid. If they do not, the interpolation will not be valid, and the program will exit with an error.
