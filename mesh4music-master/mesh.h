
#pragma once
#ifndef MESH_H
#define MESH_H

#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "triangle.h"
#endif

/*****************************************************************************/
/*                                                                           */
/* a mesh data structure, mostly a copy form the one returned by triangle    */
/*                                                                           */
/*****************************************************************************/

/** A point in 2 dimension
 */
struct vec2 {
    REAL x, y;
};

/** Vertices (id) of a triangle
 */
struct triangle {
    int v_id[3];
};

/** Vertices (id) of an edge
 */
struct edge {
    int v_id[2];
};

/** A mesh in 2 dimension
 */
struct Mesh {
    int n_points; // number of points
    int n_triangles; // number of triangles
    int n_edges;

    struct vec2* v_points;
    
    struct triangle *v_triangles;
    struct edge *v_edges;

    int *v_boundary_id; // 1 value per vertex, val of 0 means not on boundary
    
    int nmax; //maximum number of neighbors per vertex
    int *nbNeighbors; // 1 value per vertex, numbers of neighbors
    int *neighbors;   // id of neighbors
    REAL *barycoord;
    // we end up with some unused memory, but we have direct access to data of vertex i :
    //      neighbors[i*nmax + j] => id of j-th neighbors of vertex i
    //      barycoord[i*nmax + j] => associated barycentric coordinates
    // NB : we could have store pointer to struct instead, but this would have required more
    // pointer management and memory allocations

    
    // we can also store precomputed matrices here (avoid a large amount of redundant
    // computation at the expense of memory)

    REAL *gradOp;

};

/** Initialize mesh structure from triangle output */
void moveAndInit(struct triangulateio *in, struct Mesh *mesh);

/** Computation of generalized barycentric coordinates */
void computeMeanValueCoordinates(struct Mesh *mesh);

void computeGradientOperators(struct Mesh *mesh);

/** Compute the multiplication A*B and set the result in R.
 *
 *  All matrices are stored in row major format.
 *
 *  A is of size (n_row,n)
 *  B is of size (n, n_col)
 *  R is of size (n_row,n_col)
 *
 *  The memory of R should already be allocated.
 */
void matrixMult(REAL *A, REAL *B, REAL *R, int n_row, int n_col, int n);

/** Compute the matrix operator corresponding to the gradient of vertex i
 *
 *  All matrices are stored in row major format and there memory should be
 *  already allocated.
 *
 *  opM will contains the result, it should be able to represent a matrix up to size (2,nmax+1)
 *
 *  B should be able to represent a matrix up to size (2,nmax+1)
 */
void buildOpGrad(struct Mesh *mesh, int i, REAL *opM, REAL *B);

void saveBinaryMesh(struct Mesh *m, const char* filename);


#endif
