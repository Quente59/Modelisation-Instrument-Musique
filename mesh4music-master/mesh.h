
/* If SINGLE is defined when triangle.o is compiled, it should also be       */
/*   defined here.  If not, it should not be defined here.                   */

/* #define SINGLE */

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

struct Mesh {
    REAL *pointlist;                                               /* In / out */
    REAL *pointattributelist;                                      /* In / out */
    int *pointmarkerlist;                                          /* In / out */
    int numberofpoints;                                            /* In / out */
    int numberofpointattributes;                                   /* In / out */
    
    int *trianglelist;                                             /* In / out */
    REAL *triangleattributelist;                                   /* In / out */
    REAL *trianglearealist;                                         /* In only */
    int *neighborlist;                                             /* Out only */
    int numberoftriangles;                                         /* In / out */
    int numberofcorners;                                           /* In / out */
    int numberoftriangleattributes;                                /* In / out */
    
    int *segmentlist;                                              /* In / out */
    int *segmentmarkerlist;                                        /* In / out */
    int numberofsegments;                                          /* In / out */
    
    int *edgelist;                                                 /* Out only */
    int *edgemarkerlist;            /* Not used with Voronoi diagram; out only */
    int numberofedges;
    
    
    int *numberofNeighbors;
    int *neighbors;
    REAL *barycoord;
    int nmax; //maximum number of neighbors per vertex
    // we end up with some unused memory, but we have direct access to data of vertex i :
    //      neighbors[i*nmax + j] => id of j-th neighbors of vertex i
    //      barycoord[i*nmax + j] => associated barycentric coordinates
    // NB : we could have store pointer to struct instead, but this would have required more
    // pointer management and memory allocations
    
    // we could also store precomputed matrices here (avoid a large amount of redundant computation at the expense of
    // memory)
    
};

/** Initialize mesh structure from triangle output */
void moveAndInit(struct triangulateio *in, struct Mesh *mesh);

/** Computation of generalized barycnetric coordinates */
void computeMeanValueCoordinates(struct Mesh *mesh);

