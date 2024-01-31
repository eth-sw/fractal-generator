#ifndef MAIN_ASSIGNMENT_STRUCTURES_H
#define MAIN_ASSIGNMENT_STRUCTURES_H

// Define maximum lengths and limits
#define MAX_NAME_LENGTH 100
#define MAX_TRANSFORMS 100
#define MAX_GRAPHIC 100
#define MAX_FRACTALS 100
#define MAX_BRANCHES 100
#define MAX_COORDINATES 10

/**
 * Structure for Translation coordinates
 */
typedef struct {
    float x;
    float y;
} Translation;

/**
 * Structure representing a Transformation
 */
typedef struct {
    char name[MAX_NAME_LENGTH];
    double rotation;
    double translationX;
    double translationY;
    double scale;
    Translation translation;
} Transform;

/**
 * Structure representing a Graphic with associated Transformation
 */
typedef struct {
    char name[MAX_NAME_LENGTH];
    int coordinateCount;
    double pointX[MAX_COORDINATES];
    double pointY[MAX_COORDINATES];
    double coordinates[MAX_COORDINATES][2];
    Transform associatedTransform;
} Graphic;

/**
 * Structure representing a Branch instruction
 */
typedef struct {
    char transformName[MAX_NAME_LENGTH];
    int rangeStart;
    int rangeEnd;
    char instructionType[MAX_NAME_LENGTH];
    char instructionName[MAX_NAME_LENGTH];
} Branch;

/**
 * Structure representing a Fractal with an associated Branch
 */
typedef struct {
    char name[MAX_NAME_LENGTH];
    Branch associatedBranch;
} Fractal;


/**
 * Structure representing all instruction types with counts and arrays
 */
typedef struct {
    Transform transforms[MAX_TRANSFORMS];
    Graphic graphics[MAX_GRAPHIC];
    Fractal fractals[MAX_FRACTALS];
    Branch branches[MAX_BRANCHES];
    int transformCount;
    int graphicCount;
    int branchCount;
    int fractalCount;
} AllInstructions;

#endif