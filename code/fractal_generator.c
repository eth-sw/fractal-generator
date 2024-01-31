#include "structures.h"
#include "fractal_generator.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int count = 1;

/**
 * Function to rotate a translation vector or a coordinate (rotated around the origin)
 * @param rotation_angle The rotation angle in degrees
 * @param tr The translation or coordinate to be rotated
 * @return New Translation is returned - caller must free the memory allocated
 */
Translation *rotate(double rotation_angle, Translation* tr) {
    // Create new Translation to hold transformed point
    Translation *result_tr = (Translation*)malloc(sizeof(Translation));

    // Convert rotation to radians
    double radians = rotation_angle * (M_PI / 180);

    // Apply rotation
    result_tr->x = tr->x * cos(radians) - tr->y * sin(radians);
    result_tr->y = tr->x * sin(radians) + tr->y * cos(radians);

    return result_tr;
}

/**
 * Function to create new global branch_translation by applying a global transform
 * to a branch translation
 * @param global_rotation Rotation required in degrees
 * @param global_scale  Scale required (<1 shrinks, 1 maintains size, >1 enlarges)
 * @param global_translation Translation
 * @param branch_translation  Fractal branch to modify to the global translation
 * @return New Translation is returned - caller must free the memory allocated
 */
Translation *apply_transform_to_translation(double global_rotation, double global_scale, Translation global_translation, Translation branch_translation) {
    // Produce new global_translation by rotating branch_translation
    Translation *new_translation = rotate(global_rotation, &branch_translation);

    // Scale new branch_translation
    new_translation->x = new_translation->x * global_scale;
    new_translation->y = new_translation->y * global_scale;

    // Add rotated and scaled branch_translation to the transform
    new_translation->x += global_translation.x;
    new_translation->y += global_translation.y;

    return new_translation;
}

/**
 * Recursively apply transforms to generate the pattern
 * Repeats for number of shapes specified by the branch range
 * @param svgFile Output SVG file to write generated pattern
 * @param instruction AllInstructions struct
 * @param transform Transform struct
 * @param graphic Graphic struct
 * @param branch Branch struct
 * @param depth Depth of recursion
 * @param parentScale Scale of parent shape
 * @param parentTranslationX X translation of parent shape
 * @param parentTranslationY Y translation of parent shape
 * @param cumulativeRotation Cumulative rotation of parent shape
 * @param shapeCount Number of shapes generated so far
 * @param maxShapes Maximum number of shapes to generate
 */
void applyTransformRecursive(FILE *svgFile, AllInstructions *instruction, Transform *transform, Graphic *graphic, Branch *branch, int depth, double parentScale, double parentTranslationX, double parentTranslationY, double cumulativeRotation, int *shapeCount, int maxShapes) {
    // Check if max number of shapes has been reached
    if (*shapeCount >= maxShapes) {
        return;
    }

    // Update cumulative rotation
    cumulativeRotation += transform->rotation;

    // Apply global transform to branch translation
    Translation translation = { transform->translationX, transform->translationY };
    Translation transformedTranslation = *apply_transform_to_translation(cumulativeRotation - transform->rotation, parentScale, (Translation){parentTranslationX, parentTranslationY}, translation);

    // Calculate new scale
    double newScale = transform->scale * parentScale;

    // Calculate stroke width based on parent scale
    double strokeWidth = 1 / parentScale;

    // Apply transform to graphic
    fprintf(svgFile, "<g transform=\"translate(%.6lf,%.6lf) rotate(%.6lf) scale(%.6lf)\">\n",
            transformedTranslation.x + 350, -transformedTranslation.y + 350, -cumulativeRotation, newScale);

    // Use graphic coordinates to draw shape
    fprintf(svgFile, "<polyline points=\"");
    for (int k = 0; k < graphic->coordinateCount; k++) {
        fprintf(svgFile, "%.6lf,%.6lf ", graphic->coordinates[k][0], -graphic->coordinates[k][1]);
    }
    fprintf(svgFile, "\" fill=\"none\" stroke=\"black\" stroke-width=\"%.6lf\"/>\n", strokeWidth);

    // Close group
    fprintf(svgFile, "</g>\n");

    // Check if max number of shapes has been reached
    if (count == branch->rangeEnd) {
        // Apply the global transformation for first shape
        fprintf(svgFile, "<g transform=\"translate(350,350) rotate(0) scale(1)\">\n");

        // Draw initial shape using first graphic specification
        fprintf(svgFile, "<polyline points=\"");
        for (int k = 0; k < instruction->graphics[0].coordinateCount; k++) {
            fprintf(svgFile, "%.6lf,%.6lf ", instruction->graphics[0].coordinates[k][0], -instruction->graphics[0].coordinates[k][1]);
        }
        fprintf(svgFile, "\" fill=\"none\" stroke=\"black\"/>\n");
        // Close the group for the initial shape
        fprintf(svgFile, "</g>\n");
    }

    count++; // Increment count

    *shapeCount += 1;  // Increment shape count

    // Use branch range to calculate the number of repetitions
    int repetitions = branch->rangeEnd - branch->rangeStart + 1;

    // Generate shapes recursively
    for (int i = 0; i < repetitions; i++) {
        applyTransformRecursive(svgFile, instruction, transform, graphic, branch, depth - 1, newScale, transformedTranslation.x, transformedTranslation.y, cumulativeRotation, shapeCount, maxShapes);

        // Update cumulative rotation
        cumulativeRotation += transform->rotation;
    }
}

/**
 * Generates the fractal pattern
 * Checks if there is a fractal in the NFSF file, if there is then it generates the first graphic
 * Then calls applyTransformRecursive() to continue generating the fractal
 * @param svgFile Output SVG file to write generated pattern
 @param instruction AllInstructions struct
 * @param transform Transform struct
 * @param fractal
 * @param transforms
 * @param graphic
 * @param graphic Graphic struct
 * @param branch Branch struct
 */
void generateFractal(FILE *svgFile, AllInstructions *instruction, Fractal *fractal, Transform *transform, Graphic *graphic, Branch *branch) {
    // Find index of fractal in array
    int fractalIndex = -1;
    for (int i = 0; i < MAX_FRACTALS; i++) {
        if (strcmp(fractal[i].name, "") != 0) {
            fractalIndex = i;
            break;
        }
    }

    // Apply transforms and graphics based on the specified fractal and branches
    if (fractalIndex != -1) {
        // Set to one as the initial shape is already drawn
        int shapeCount = 1;

        // Output initial transform as a separate shape
        fprintf(svgFile, "<g transform=\"translate(%.6lf,%.6lf) rotate(%.6lf) scale(%.6lf)\">\n",
                transform[fractalIndex].translationX + 350, -transform[fractalIndex].translationY + 350,
                -transform[fractalIndex].rotation, transform[fractalIndex].scale);

        // Use graphic coordinates to draw shape
        fprintf(svgFile, "<polyline points=\"");
        for (int k = 0; k < graphic[fractalIndex].coordinateCount; k++) {
            fprintf(svgFile, "%.6lf,%.6lf ", graphic[fractalIndex].coordinates[k][0], -graphic[fractalIndex].coordinates[k][1]);
        }
        fprintf(svgFile, "\" fill=\"none\" stroke=\"black\" stroke-width=\"%.6lf\"/>\n", 1.0);

        // Close group
        fprintf(svgFile, "</g>\n");

        // Start recursively applying transforms to generate the pattern
        applyTransformRecursive(svgFile, instruction, &transform[fractalIndex], &graphic[fractalIndex], &branch[fractalIndex], branch[fractalIndex].rangeEnd + 1, transform->scale, transform->translationX, transform->translationY, transform->rotation, &shapeCount, branch[fractalIndex].rangeEnd);

        // Add fractal to list of fractals used
        instruction->fractals[instruction->fractalCount++] = fractal[fractalIndex];
    }
}
