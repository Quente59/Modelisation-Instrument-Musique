#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif

#include <stdio.h>
#include <stlib.h>

#include <math.h>
#include <assert.h>

#include <mesh.h>

#define freq 44100
#define duration 1

struct Mesh *mesh;

REAL *uc, *up, *us;
REAL *d4uc, *d4up;


