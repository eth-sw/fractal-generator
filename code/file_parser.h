#ifndef MAIN_ASSIGNMENT_FILE_PARSER_H
#define MAIN_ASSIGNMENT_FILE_PARSER_H

#include "structures.h"

// Define how many chars a line can be
#define MAX_LINE_LENGTH 256

int parseTransform(const char *line, AllInstructions *instruction, Transform *transform);
int parseGraphic(const char *line, AllInstructions *instruction, Graphic *graphic);
int parseFractal(const char *line, AllInstructions *instruction, Fractal *fractal);
int parseBranch(const char *line, AllInstructions *instruction, Branch *branch);
int parseNFSFFile(const char *filename, AllInstructions *instruction, Transform *transform, Graphic *graphic, Fractal *fractal, Branch *branch);

#endif //MAIN_ASSIGNMENT_FILE_PARSER_H
