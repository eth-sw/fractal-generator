# Fractal Colouring Book Generator

This is my implementation of the Fractal Colouring Book Generator assignment for the CS23820 C and C++ module. It is a program written in C that allows users to create fractal patterns based on user-defined specifications stored in an NFSF (Neals Fractal Specification Format) input file. The corresponding SVG output file is then generated.

## File Structure

- `main.c`: The starting file for the program, provides the user with a textual interface to perform further actions.
- `file_parser.c` and `file_parser.h`: Contains functions for parsing NFSF files, storing the parsed data in structures stored in 'structures.h'
- `fractal_generator.c` and `fractal_generator.h`: Contains functions for generating fractals recursively from the parsed data stored in 'structures.h', and outputs them to the SVG file.
- `structures.h`: Defines the data structures used in the program.

## Building the Code

To build the Fractal Colouring Book Generator, follow these steps:

### 1. via the CLion IDE

#### Prerequisites

- CLion IDE

#### Build Instructions

1. Open the project in CLion.
2. Build the project using the build tools in the IDE.

#### Execution

- Run the compiled code from within CLion.

### 2. via the Terminal

#### Prerequisites

- C compiler (e.g., GCC)
- Standard Template Library (STL) support for C

#### Build Commands

gcc main.c file_parser.c fractal_generator.c -o fractal_generator -lm

#### Execution

./fractal_generator

## Usage

1. Run the compiled executable.
2. Enter the NFSF file name when prompted.
3. Enter the SVG output file name when prompted.
4. Open the output SVG file containing the fractal.

## Dependencies

- The program relies on the Standard Template Library (STL) for certain functionalities.
- Cygwin: The program is developed and tested using Cygwin on Windows.

## Libraries/Packages

- stdio.h
- string.h
- stdlib.h
- math.h

## Version

This README corresponds to version 1.0 of the Fractal Colouring Book Generator.
