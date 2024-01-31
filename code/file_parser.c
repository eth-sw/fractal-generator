#include "structures.h"
#include "file_parser.h"
#include <stdio.h>
#include <string.h>

/**
 * Parse TRANSFORM instruction
 * Store values in AllInstructions and Transform structs
 * @param line Line containing TRANSFORM instruction
 * @param instruction AllInstructions struct to store parsed info
 * @param transform Transform struct to store parsed info
 * @return 1 if parse successful, 0 if not
 */
int parseTransform(const char *line, AllInstructions *instruction, Transform *transform) {
    // Parse TRANSFORM instruction
    if (sscanf(line, " TRANSFORM %s ROTATION %lf TRANSLATION (%lf,%lf) SCALE %lf",
               transform->name, &transform->rotation, &transform->translationX, &transform->translationY, &transform->scale) != 5) {
        // Error handling for incorrect format
        fprintf(stderr, "Error: Cannot parse TRANSFORM instruction from line: %s\n", line);
        return 0;
    }

    // Check if there is space in AllInstructions struct to store graphic
    if (instruction->transformCount < MAX_GRAPHIC) {
        // Copy values to the AllInstructions array
        strncpy(instruction->transforms[instruction->transformCount].name, transform->name, MAX_NAME_LENGTH);
        instruction->transforms[instruction->transformCount].rotation = transform->rotation;
        instruction->transforms[instruction->transformCount].translationX = transform->translationX;
        instruction->transforms[instruction->transformCount].translationY = transform->translationY;
        instruction->transforms[instruction->transformCount].scale = transform->scale;

        // Increment the transformCount
        instruction->transformCount++;
    } else {
        fprintf(stderr, "Error: Max number of transforms exceeded\n");
        return 0;
    }

    return 1;

}



/**
 * Parse GRAPHIC instruction
 * Store values in AllInstructions and Graphic structs
 * @param line Line containing GRAPHIC instruction
 * @param instruction AllInstructions struct to store parsed info
 * @param graphic Graphic struct to store parsed info
 * @return 1 is parse successful, 0 if not
 */
int parseGraphic(const char *line, AllInstructions *instruction, Graphic *graphic) {
    int coordinateCount = 0; // Counter for coordinates

    // Parse GRAPHIC instruction name
    if (sscanf(line, " GRAPHIC %s", graphic->name) != 1) {
        // Error handling for incorrect format
        fprintf(stderr, "Error: Cannot parse GRAPHIC instruction from line: %s\n", line);
        return 0;
    }

    // Find the start of the coordinates
    const char *coordinateStart = strstr(line, "(");

    // Parse the coordinates
    while (sscanf(coordinateStart, " (%lf,%lf)", &graphic->coordinates[coordinateCount][0], &graphic->coordinates[coordinateCount][1]) == 2) {
        coordinateCount++;
        // Check if max number of coordinates is exceeded
        if (coordinateCount == MAX_COORDINATES) {
            fprintf(stderr, "Error: Max number of coordinates exceeded\n");
            return 0;
        }
        // Go to next coordinate
        coordinateStart = strstr(coordinateStart + 1, "(");
    }

    // Update coordinate count in Graphic struct
    graphic->coordinateCount = coordinateCount;

    // Check if there is space in AllInstructions struct to store graphic
    if (instruction->graphicCount < MAX_GRAPHIC) {
        // Copy graphic name to AllInstructions struct
        strncpy(instruction->graphics[instruction->graphicCount].name, graphic->name, MAX_NAME_LENGTH);
        instruction->graphics[instruction->graphicCount].coordinateCount = graphic->coordinateCount;
        // Copy coordinate values to AllInstructions struct
        for (int i = 0; i < coordinateCount; i++) {
            instruction->graphics[instruction->graphicCount].coordinates[i][0] = graphic->coordinates[i][0];
            instruction->graphics[instruction->graphicCount].coordinates[i][1] = graphic->coordinates[i][1];
        }

        // Increment the graphicCount
        instruction->graphicCount++;
    }
    else {
        fprintf(stderr, "Error: Max number of graphics exceeded\n");
        return 0;
    }

    return 1;
}

/**
 * Parse FRACTAL instruction
 * Store values in AllInstructio1ns and Fractal structs
 * @param line Line containing FRACTAL instruction
 * @param instruction AllInstructions struct to store parsed info
 * @param fractal Fractal struct to store parsed values
 * @return 1 if parse successful, 0 if not
 */
int parseFractal(const char *line, AllInstructions *instruction, Fractal *fractal) {
    // Parse FRACTAL instruction
    if (sscanf(line, " FRACTAL %s", fractal->name) != 1) {
        fprintf(stderr, "Error: Cannot parse FRACTAL instruction from line: %s\n", line);
        return 0;
    }

    // Check if there is space in AllInstructions struct to store graphic
    if (instruction->fractalCount < MAX_GRAPHIC) {
        // Copy values to the AllInstructions array
        strncpy(instruction->fractals[instruction->fractalCount].name, fractal->name, MAX_NAME_LENGTH);
    } else {
        // Error handling
        fprintf(stderr, "Error: Max number of fractals exceeded\n");
        return 0;
    }

//    Branch *associatedBranch = NULL;
//    for (int i = 0; i < instruction->branchCount; i++) {
//        if (strcmp(instruction->branches[i].instructionName, fractal->name) == 0) {
//            associatedBranch = &instruction->branches[i];
//            break;
//        }
//    }
//
//    if (associatedBranch == NULL) {
//        fprintf(stderr, "Error: Branch not found for fractal: %s\n", fractal->name);
//        return 0;
//    }

    return 1;
}

/**
 * Parse BRANCH instruction
 * Store values in AllInstructions and Branch structs
 * @param line Line containing BRANCH instruction
 * @param instruction AllInstructions struct to store parsed info
 * @param branch Branch struct to store parsed values
 * @return 1 if parse successful, 0 if not
 */
int parseBranch(const char *line, AllInstructions *instruction, Branch *branch) {
    // Parse BRANCH instruction
    if (sscanf(line, " BRANCH %s [%d:%d] %s %s", branch->transformName, &branch->rangeStart, &branch->rangeEnd, branch->instructionType, branch->instructionName) != 5) {
        // Error handling
        fprintf(stderr, "Error: Cannot parse BRANCH instruction from line: %s\n", line);
        return 0;
    }

    // Check if there is space in AllInstructions struct to store graphic
    if (instruction->branchCount < MAX_GRAPHIC) {
        // Copy values to the AllInstructions array
        strncpy(instruction->branches[instruction->branchCount].transformName, branch->transformName, MAX_NAME_LENGTH);
        instruction->branches[instruction->branchCount].rangeStart = branch->rangeStart;
        instruction->branches[instruction->branchCount].rangeEnd = branch->rangeEnd;
        strncpy(instruction->branches[instruction->branchCount].instructionType, branch->instructionType, MAX_NAME_LENGTH);
        strncpy(instruction->branches[instruction->branchCount].instructionName, branch->instructionName, MAX_NAME_LENGTH);

        // Increment the branchCount
        instruction->branchCount++;
    } else {
        fprintf(stderr, "Error: Max number of branches exceeded\n");
        return 0;
    }

    // Find the associated transform for the branch
    Transform *associatedTransform = NULL;
    for (int i = 0; i < instruction->transformCount; i++) {
        if (strcmp(instruction->transforms[i].name, branch->transformName) == 0) {
            associatedTransform = &instruction->transforms[i];
            break;
        }
    }

    if (associatedTransform == NULL) {
        fprintf(stderr, "Error: Transform not found for branch: %s\n", branch->transformName);
        return 0;
    }

    // Find the associated graphic for the branch
    Graphic *associatedGraphic = NULL;
    for (int i = 0; i < instruction->graphicCount; i++) {
        if (strcmp(instruction->graphics[i].name, branch->instructionName) == 0) {
            associatedGraphic = &instruction->graphics[i];
            break;
        }
    }

    if (associatedGraphic == NULL) {
        fprintf(stderr, "Error: Graphic not found for branch: %s\n", branch->instructionName);
        return 1;
    }

    // Associate the graphic with the found transform
    associatedGraphic->associatedTransform = *associatedTransform;

//    Fractal *associatedFractal = NULL;
//    for (int i = 0; i < instruction->fractalCount; i++) {
//        if (strcmp(instruction->fractals[i].name, branch->instructionName) == 0) {
//            associatedFractal = &instruction->fractals[i];
//            break;
//        }
//    }
//
//    if (associatedFractal != NULL) {
//        associatedFractal->associatedTransform = *associatedTransform;
//    } else {
//        fprintf(stderr, "Error: Fractal not found for branch: %s\n", branch->instructionName);
//        return 0;
//    }

    return 1;
}

/**
 * Parse NFSF file
 * @param filename Input filename
 * @param instruction AllInstructions struct
 * @param transform Transform struct
 * @param graphic Graphic struct
 * @param fractal Fractal struct
 * @param branch Branch struct
 * @return 1 if parse successful, 0 if not
 */
int parseNFSFFile(const char *filename, AllInstructions *instruction, Transform *transform, Graphic *graphic, Fractal *fractal, Branch *branch) {
    // Open file for reading
    FILE *file = fopen(filename, "r");

    // Check if file is opened successfully
    if(file == NULL) {
        fprintf(stderr, "Error: Cannot open file: %s\n", filename);
        return 0;
    }

    // Variables to store each line and command
    char line[MAX_LINE_LENGTH];
    char command[MAX_NAME_LENGTH];

    // Loop through each line of file
    while(fgets(line, sizeof(line), file) != NULL) {
        // Remove newline characters from end of line
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[len - 1] = '\0';
        }

        // Skip empty lines
        if (line[0] == '\0') {
            continue;
        }

        // Skips comment lines
        if (line[0] == '/' && line[1] == '/') {
            continue;
        }

        // Extract command from line
        if(sscanf(line, "%s", command) != 1) {
            fprintf(stderr, "Error: Cannot parsed command from line: %s\n", line);
            fclose(file);
            return 0;
        }

        // Call parsing function based on command
        if (strcmp(command, "TRANSFORM") == 0) { // Parse TRANSFORM instruction
            if (!parseTransform(line, instruction, transform)) {
                fclose(file);
                return 0;
            }
        }
        else if (strcmp(command, "GRAPHIC") == 0) { // Parse GRAPHIC instruction
            if (!parseGraphic(line, instruction, graphic)) {
                fclose(file);
                return 0;
            }
        }
        else if (strcmp(command, "FRACTAL") == 0) { // Parse FRACTAL instruction
            if (!parseFractal(line, instruction, fractal)) {
                fclose(file);
                return 0;
            }
        }
        else if (strcmp(command, "BRANCH") == 0) { // Parse BRANCH instruction
            if (!parseBranch(line, instruction, branch)) {
                fclose(file);
                return 0;
            }
        }
        else { // If command is not recognised, print error
            fprintf(stderr, "Error: Unknown instruction: %s\n", line);
            fclose(file);
            return 0;
        }
    }

    // Close file
    fclose(file);

    return 1;
}