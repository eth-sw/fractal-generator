# Fractal Colouring Book Generator README

## Overview

The Fractal Colouring Book Generator is a C program that reads instructions from an NFSF (Fractal Specification File) file and generates an SVG (Scalable Vector Graphics) file representing a fractal pattern. The program supports transforms, graphics, fractals, and branches to create intricate and visually appealing patterns.

## Table of Contents

- [Getting Started](#getting-started)
- [Usage](#usage)
- [File Structure](#file-structure)
- [NFSF File Format](#nfsf-file-format)
- [Dependencies](#dependencies)
- [Libraries/Packages](#libraries-packages)
- [Examples](#examples)
- [License](#license)

## Getting Started

To use the Fractal Colouring Book Generator, follow these steps:

1. Clone the repository to your local machine.
2. Compile the C program using a C compiler (e.g., GCC).
3. Run the executable, providing the NFSF file as input when prompted.
4. Enter the desired output SVG file name when prompted.

## Usage

1. Run the compiled executable.
2. Enter the NFSF file name when prompted.
3. Enter the SVG output file name when prompted.
4. Open the output SVG file containing the fractal.

## File Structure

The program consists of multiple files:

- **file_parser.h:** Header file containing functions for parsing NFSF files.
- **fractal_generator.h:** Header file containing functions for generating the fractal pattern.
- **structures.h:** Header file defining the data structures used in the program.
- **main.c:** Main program file containing the user interface and main logic.

## NFSF File Format

The NFSF file is a text-based file containing instructions for the fractal generation. It includes information about transforms, graphics, fractals, and branches. The file format is structured to define the components needed to create the fractal pattern.

## Dependencies

- The program relies on the Standard Template Library (STL) for certain functionalities.
- Cygwin: The program is developed and tested using Cygwin on Windows.

## Libraries/Packages

- stdio.h
- string.h
- stdlib.h
- math.h

## Examples

The repository includes example NFSF files that demonstrate various fractal patterns. Users can explore these examples to understand the syntax and structure of NFSF files.

## License

The Fractal Colouring Book Generator is released under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code as per the terms of the license.
