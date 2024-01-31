#ifndef MAIN_ASSIGNMENT_FRACTAL_GENERATOR_H
#define MAIN_ASSIGNMENT_FRACTAL_GENERATOR_H

#include <stdio.h>
#include "structures.h"

Translation *rotate(double rotation_angle, Translation* tr);
Translation *apply_transform_to_translation(double global_rotation, double global_scale, Translation global_translation, Translation branch_translation);
void applyTransformRecursive(FILE *svgFile, AllInstructions *instruction, Transform *transform, Graphic *graphic, Branch *branch, int depth, double parentScale, double parentTranslationX, double parentTranslationY, double cumulativeRotation, int *shapeCount, int maxShapes);
void generateFractal(FILE *svgFile, AllInstructions *instruction, Fractal *fractal, Transform *transform, Graphic *graphic, Branch *branch);

#endif //MAIN_ASSIGNMENT_FRACTAL_GENERATOR_H
