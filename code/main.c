#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_parser.h"
#include "fractal_generator.h"
#include "structures.h"

#define DEFAULT_VALUE 0.0 // Used to initialise all structs as 0.0
#define MAX_FILENAME_LENGTH 100

char inputFilename[MAX_FILENAME_LENGTH];
char outputFilename[MAX_FILENAME_LENGTH];

/**
 * Retrieves the input filename from the user
 */
void retrieveInputFilename() {
    printf("Welcome to Fractal Colouring Book Generator!\n");
    printf("Enter the NFSF file name: \n");
    if (scanf("%s", inputFilename) != 1) { // Take input filename, contains error handling
        fprintf(stderr, "Error reading filename.\n");
        exit(1);
    }
    if(!strstr(inputFilename, ".txt")) { // Append .txt to filename if not already present
        strcat(inputFilename, ".txt");
    }
    printf("\n");
}

/**
 * Parses the NFSF file and store values in instruction, transform, graphic, fractal, branch structs
 * @param instruction AllInstructions struct
 * @param graphic Graphic struct
 * @param fractal Fractal struct
 */
void nfsfParser(AllInstructions* instruction, Graphic* graphic, Fractal* fractal) {
    // Ensure that all values are set to default, reduces risk of errors
    instruction->transformCount = DEFAULT_VALUE;
    instruction->graphicCount = DEFAULT_VALUE;
    instruction->branchCount = DEFAULT_VALUE;
    instruction->fractalCount = DEFAULT_VALUE;

    Transform transform;
    strcpy(transform.name, "");
    transform.rotation = DEFAULT_VALUE;
    transform.translationX = DEFAULT_VALUE;
    transform.translationY = DEFAULT_VALUE;
    transform.scale = DEFAULT_VALUE;

    strcpy(graphic->name, "");
    graphic->coordinateCount = DEFAULT_VALUE;
    graphic->pointX[0] = DEFAULT_VALUE;
    graphic->pointY[0] = DEFAULT_VALUE;
    graphic->coordinates[0][0] = DEFAULT_VALUE;
    graphic->coordinates[0][1] = DEFAULT_VALUE;

    strcpy(fractal->name, "");

    Branch branch;
    strcpy(branch.transformName, "");
    branch.rangeStart = DEFAULT_VALUE;
    branch.rangeEnd = DEFAULT_VALUE;
    strcpy(branch.instructionName, "");
    strcpy(branch.instructionType, "");

    // Parse NFSF file, store values in instruction, transform, graphic, fractal, branch structs
    if (!parseNFSFFile(inputFilename, instruction, &transform, graphic, fractal, &branch)) {
        fprintf(stderr, "Error parsing NFSF file.\n");
        exit(0);
    }
}

/**
 * Retrieves the output filename from the user
 */
void retrieveOutputFilename() {
    printf("Enter SVG output file name: \n");
    scanf("%s", outputFilename); // Take output filename
    if(!strstr(outputFilename, ".svg")) { // Append .svg to filename if not already present
        strcat(outputFilename, ".svg");
    }
    printf("\n");
}

/**
 * Generates the SVG fractal pattern
 * @param instruction AllInstructions struct
 * @param graphic Graphic struct
 * @param fractal Fractal struct
 */
void generateSVGPattern(AllInstructions* instruction, Graphic* graphic, Fractal* fractal) {
    // Open SVG file to write to
    FILE *svgFile = fopen(outputFilename, "w");

    // Check if file is opened successfully
    if (!svgFile) {
        fprintf(stderr, "Error opening SVG file: %s\n", outputFilename);
        exit(0);
    }

    // Open SVG file, set canvas size to 700x700
    fprintf(svgFile, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"700\" height=\"700\">\n");
    fprintf(svgFile, "<rect width=\"100%%\" height=\"100%%\" fill=\"white\" stroke=\"black\"/>\n");

    // Apply the global transformation for first shape
    fprintf(svgFile, "<g transform=\"translate(350,350) rotate(0) scale(1)\">\n");

    // Draw initial shape using first graphic specification
    fprintf(svgFile, "<polyline points=\"");
    for (int k = 0; k < graphic->coordinateCount; k++) {
        fprintf(svgFile, "%.6lf,%.6lf ", graphic->coordinates[k][0], -graphic->coordinates[k][1]);
    }
    fprintf(svgFile, "\" fill=\"none\" stroke=\"black\"/>\n");

    // Close group tag
    fprintf(svgFile, "</g>\n");

    // Apply the global transformation for each shape
    for (int i = 0; i < instruction->branchCount; i++) {
        generateFractal(svgFile, instruction, fractal, &instruction->transforms[i], &instruction->graphics[i],
                        &instruction->branches[i]);
    }

    // Close SVG tag
    fprintf(svgFile, "</svg>\n");

    // Close SVG file
    fclose(svgFile);
}

/**
 * Print information from structures to console
 * @param instruction AllInstructions struct
 */
void printStructInfo(AllInstructions *instruction) {
    // Print information from data structures to the console
    printf("Transforms:\n");
    for (int i = 0; i < instruction->transformCount; i++) {
        printf("Transform %d:\n", i + 1);
        printf("  Name: %s\n", instruction->transforms[i].name);
        printf("  Rotation: %.6lf\n", instruction->transforms[i].rotation);
        printf("  TranslationX: %.6lf\n", instruction->transforms[i].translationX);
        printf("  TranslationY: %.6lf\n", instruction->transforms[i].translationY);
        printf("  Scale: %.6lf\n", instruction->transforms[i].scale);
        printf("\n");
    }

    printf("Graphics:\n");
    for (int i = 0; i < instruction->graphicCount; i++) {
        printf("Graphic %d:\n", i + 1);
        printf("  Name: %s\n", instruction->graphics[i].name);
        printf("  Coordinate Count: %d\n", instruction->graphics[i].coordinateCount);
        for (int j = 0; j < instruction->graphics[i].coordinateCount; j++) {
            printf("    Coordinate %d:\n", j + 1);
            printf("      X: %.6lf\n", instruction->graphics[i].coordinates[j][0]);
            printf("      Y: %.6lf\n", instruction->graphics[i].coordinates[j][1]);
        }
        printf("  Associated Transform:\n");
        printf("    Name: %s\n", instruction->graphics[i].associatedTransform.name);
        printf("    Rotation: %.6lf\n", instruction->graphics[i].associatedTransform.rotation);
        printf("    TranslationX: %.6lf\n", instruction->graphics[i].associatedTransform.translationX);
        printf("    TranslationY: %.6lf\n", instruction->graphics[i].associatedTransform.translationY);
        printf("    Scale: %.6lf\n", instruction->graphics[i].associatedTransform.scale);
        printf("\n");
    }

    printf("Fractals:\n");
    for (int i = 0; i < instruction->fractalCount; i++) {
        printf("Fractal %d:\n", i + 1);
        printf("  Name: %s\n", instruction->fractals[i].name);
        printf("\n");
    }

    printf("Branches:\n");
    for (int i = 0; i < instruction->branchCount; i++) {
        printf("Branch %d:\n", i + 1);
        printf("  Transform Name: %s\n", instruction->branches[i].transformName);
        printf("  Range Start: %d\n", instruction->branches[i].rangeStart);
        printf("  Range End: %d\n", instruction->branches[i].rangeEnd);
        printf("  Instruction Type: %s\n", instruction->branches[i].instructionType);
        printf("  Instruction Name: %s\n", instruction->branches[i].instructionName);
        printf("\n");
    }
}

/**
 * Main function
 * @return EXIT_SUCCESS
 */
int main() {
    retrieveInputFilename(); // Retrieve input filename from user

    // Initialise structs
    AllInstructions instruction;
    Graphic graphic;
    Fractal fractal;

    nfsfParser(&instruction, &graphic, &fractal); // Parse NFSF file

    retrieveOutputFilename(); // Retrieve output filename from user

    generateSVGPattern(&instruction, &graphic, &fractal); // Generate SVG pattern

    printStructInfo(&instruction); // Print information from data structures to console

    return 1;
}

