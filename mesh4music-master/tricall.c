/*****************************************************************************/
/*                                                                           */
/*  (tricall.c)                                                              */
/*                                                                           */
/*  Example program that demonstrates how to call Triangle.                  */
/*                                                                           */
/*  Accompanies Triangle Version 1.6                                         */
/*  July 19, 1996                                                            */
/*                                                                           */
/*  This file is placed in the public domain (but the file that it calls     */
/*  is still copyrighted!) by                                                */
/*  Jonathan Richard Shewchuk                                                */
/*  2360 Woolsey #H                                                          */
/*  Berkeley, California  94705-1927                                         */
/*  jrs@cs.berkeley.edu                                                      */
/*                                                                           */
/*****************************************************************************/

/* If SINGLE is defined when triangle.o is compiled, it should also be       */
/*   defined here.  If not, it should not be defined here.                   */

/* #define SINGLE */

#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

#include <stdio.h>
#include <stdlib.h>

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "triangle.h"
#endif

#include "triangleIO.h"
#include "mesh.h"

#include <math.h>
#include <assert.h>

/*****************************************************************************/
/*                                                                           */
/* Our main function                                                         */
/*                                                                           */
/*****************************************************************************/

int main()
{
    // Define an input and output data structure for triangle
    struct triangulateio in, out;
    
    
    // Read formated input representing the contour
    
    FILE * pFile = fopen ("contour.txt","r");
    
    int nb_point_exterior, nb_point_cavity;
    
    fscanf (pFile, "%i %i\n", &nb_point_exterior, &nb_point_cavity);
    printf("%i %i\n",nb_point_exterior, nb_point_cavity); //TODO: a remplacer
    
    // read exterior contour
    
    char test1;
    fscanf (pFile, "%c\n", &test1);
    assert(test1=='#');
    
    float x,y;
    for(int i=0; i<nb_point_exterior; ++i) {
        fscanf (pFile, "%f %f\n", &x, &y);
        printf("%f %f\n",x, y); //TODO: a remplacer
    }
    
    // read contour of cavity
    
    char test2;
    fscanf (pFile, "%c\n", &test2);
    assert(test2=='#');
    
    for(int i=0; i<nb_point_cavity; ++i) {
        fscanf (pFile, "%f %f\n", &x, &y);
        printf("%f %f\n",x, y); //TODO: a remplacer
    }
    
    fclose (pFile);
    
    
    //TODO a changer :you should first initialize those values
    //  in.numberofpoints
    //  in.pointlist
    
    
    //TODO value to modify after first step is working
    in.pointmarkerlist = (int *) malloc(in.numberofpoints * sizeof(int));
    for(int i=0; i<in.numberofpoints; i++) {
        in.pointmarkerlist[i] = 0;
    }
    in.numberofsegments = 0;
    in.numberofholes = 0;
    
    
    
    in.numberofpointattributes = 0;
    in.pointattributelist = (REAL *) NULL;
    
    // Can be ignored safely
    in.numberofregions = 1;
    in.regionlist = (REAL *) malloc(in.numberofregions * 4 * sizeof(REAL));
    in.regionlist[0] = 0.5;
    in.regionlist[1] = 5.0;
    in.regionlist[2] = 7.0;            /* Regional attribute (for whole mesh). */
    in.regionlist[3] = 0.1;          /* Area constraint that will not be used. */
    
    
    printf("Input point set:\n\n");
    report(&in, 1, 0, 0, 0, 0, 0);
    
    /* Make necessary initializations so that Triangle can return a triangulation in out */
    
    out.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
    /* Not needed if -N switch used or number of point attributes is zero: */
    out.pointattributelist = (REAL *) NULL;
    out.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
    out.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
    /* Not needed if -E switch used or number of triangle attributes is zero: */
    out.triangleattributelist = (REAL *) NULL;
    out.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
    /* Needed only if segments are output (-p or -c) and -P not used: */
    out.segmentlist = (int *) NULL;
    /* Needed only if segments are output (-p or -c) and -P and -B not used: */
    out.segmentmarkerlist = (int *) NULL;
    out.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
    out.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */
    
    triangulate("pcze", &in, &out, (struct triangulateio *) NULL);
    
    // Allow the use of program showme to visualize results
    saveToFiles(&out);
    //    report(&out, 1, 1, 1, 1, 1, 0);
    
    
    // Build the mesh data-structure
    
    struct Mesh mesh;
    moveAndInit(&out, &mesh);
    
    // Initialize value of a test function
    
    for(int i = 0; i<mesh.numberofpoints; ++i )
    {
        // Initialize function values (use a function that you know how to differentiate analitycally)
    }
    
    // Compute gradient
    
    for(int i = 0; i<mesh.numberofpoints; ++i )
    {
        // Build Matrix (NB : could be stored in mesh for later use)
        // Compute Gradient
        // Compare to closed-form gradient
    }
    
    // Free all the memory
    free(in.pointlist);
    free(in.pointattributelist);
    free(in.pointmarkerlist);
    free(in.regionlist);
    free(mesh.pointlist);
    free(mesh.pointattributelist);
    free(mesh.pointmarkerlist);
    free(mesh.trianglelist);
    free(mesh.triangleattributelist);
    free(mesh.trianglearealist);
    free(mesh.neighborlist);
    free(mesh.segmentlist);
    free(mesh.segmentmarkerlist);
    free(mesh.edgelist);
    free(mesh.edgemarkerlist);
    free(mesh.numberofNeighbors);
    free(mesh.neighbors);
    free(mesh.barycoord);
    
    return 0;
}

