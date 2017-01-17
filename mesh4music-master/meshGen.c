/*****************************************************************************/
/*                                                                           */
/*  (meshGen.c)                                                              */
/*                                                                           */
/*****************************************************************************/

#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

#include <stdio.h>
#include <stdlib.h>

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "triangle.h" // this file is NOT in the public domains !!!!
/*  Jonathan Richard Shewchuk                                                */
/*  2360 Woolsey #H                                                          */
/*  Berkeley, California  94705-1927                                         */
/*  jrs@cs.berkeley.edu                                                      */
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

    in.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
    /* Not needed if -N switch used or number of point attributes is zero: */
    in.pointattributelist = (REAL *) NULL;
    in.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
    in.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
    /* Not needed if -E switch used or number of triangle attributes is zero: */
    in.triangleattributelist = (REAL *) NULL;
    in.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
    /* Needed only if segments are output (-p or -c) and -P not used: */
    in.segmentlist = (int *) NULL;
    /* Needed only if segments are output (-p or -c) and -P and -B not used: */
    in.segmentmarkerlist = (int *) NULL;
    in.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
    in.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */
    in.trianglearealist = (REAL *) NULL;
    in.holelist = (REAL *) NULL;
    in.regionlist = (REAL *) NULL;
    
    // Read formated input representing the contour
    
    FILE * pFile = fopen ("contour.txt","r");
    
    int nb_point_exterior, nb_point_cavity;
    
    fscanf (pFile, "%i %i\n", &nb_point_exterior, &nb_point_cavity);

////////////////////////////////////////////////////////////////////////////////////
    //TODO a changer :you should first initialize those values
    in.numberofpoints = nb_point_exterior + nb_point_cavity;
    in.pointlist = (REAL*) malloc(2*in.numberofpoints*sizeof(REAL));
    printf("%i %i\n",nb_point_exterior, nb_point_cavity); //TODO STEP 1 & ?: a remplacer
////////////////////////////////////////////////////////////////////////////////////
    
    // read exterior contour

    char test1;
    fscanf (pFile, "%c\n", &test1);
    assert(test1=='#');
    
    REAL x,y;
    for(int i=0; i<nb_point_exterior; ++i) {
        fscanf (pFile, "%lf %lf\n", &x, &y);
	in.pointlist[2*i] = x;
	in.pointlist[2*i+1] = y;
    }
////////////////////////////////////////////////////////////////////////////////////
   //TODO : renormalise guitar
////////////////////////////////////////////////////////////////////////////////////
    
    
    // read contour of cavity
    
    char test2;
    fscanf (pFile, "%c\n", &test2);
    assert(test2=='#');
    
    struct vec2 hole_center; // coordinates of a point inside cavity
    hole_center.x = 0.0;
    hole_center.y = 0.0;

    for(int i=0; i<nb_point_cavity; ++i) {
        fscanf (pFile, "%lf %lf\n", &x, &y);
	in.pointlist[2*(i+nb_point_exterior)] = x;
	in.pointlist[2*(i+nb_point_exterior)+1] = y;

	hole_center.x += x;
	hole_center.y += y;
    }

hole_center.x /= (REAL) (nb_point_cavity);
hole_center.y /= (REAL) (nb_point_cavity);

//REnormailsation
REAL min = in.pointlist[0];
REAL max = in.pointlist[0];

for (int i=1; i<nb_point_exterior; ++i) {
	if (in.pointlist[2*i]<min) {
		min = in.pointlist[2*i];
	} if (in.pointlist[2*i]>max) {
		max = in.pointlist[2*i];
	}
}

REAL coef_norm = 0.5/(max-min);
//REAL *norm_pointlist = (REAL*) malloc(2*nb_point_exterior*sizeof(REAL));
//for (int i=1; i<2*nb_point_exterior; ++i) {
//	norm_pointlist[i] = coef_norm*in.pointlist[i];
//	in.pointlist[i] = norm_pointlist[i];
//}
//CZ 
for (int i=0; i<2*in.numberofpoints; ++i) {
    in.pointlist[i] *= coef_norm;
}
hole_center.x *= coef_norm;
hole_center.y *= coef_norm;


////////////////////////////////////////////////////////////////////////////////////
        printf("%f %f\n",x, y); //TODO STEP2: a remplacer
////////////////////////////////////////////////////////////////////////////////////
    
    
    fclose (pFile);



////////////////////////////////////////////////////////////////////////////////////
//TODO STEP3
    in.numberofsegments = in.numberofpoints;
    in.segmentlist = (REAL*) malloc(2*in.numberofpoints*sizeof(REAL));

for(int i=0; i<nb_point_exterior; ++i) {
	in.segmentlist[2*i] = i;
	in.segmentlist[2*i+1] = i+1;
	
}


in.segmentlist[2*(nb_point_exterior-1)] = nb_point_exterior-1;
in.segmentlist[2*(nb_point_exterior-1)+1] = 0;


for(int i=0; i<nb_point_cavity; ++i) {
	in.segmentlist[2*(i + nb_point_exterior)] = i + nb_point_exterior;
	in.segmentlist[2*(i + nb_point_exterior)+1] = i + nb_point_exterior + 1;


}

in.segmentlist[2*(nb_point_cavity - 1 + nb_point_exterior)] = nb_point_cavity - 1 + nb_point_exterior;
in.segmentlist[2*(nb_point_cavity - 1 + nb_point_exterior) + 1] = nb_point_exterior;


////////////////////////////////////////////////////////////////////////////////////
    
////////////////////////////////////////////////////////////////////////////////////
//TODO STEP4
    //TODO value to modify after first step is working (usefull to know which
    // point belong to boundary)
    in.pointmarkerlist = (int *) calloc(in.numberofpoints,sizeof(int));
    for(int i=0; i<nb_point_exterior; i++) {
        in.pointmarkerlist[i] = 2;
    }
    
    if(nb_point_cavity>0){
	for(int i=0; i<nb_point_cavity; i++) {
            in.pointmarkerlist[nb_point_exterior+i] = 3;
	}
    }


////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////
    //TODO Step 5
    in.numberofholes = 1;
    in.holelist = (REAL *) malloc(2*sizeof(REAL));
    in.holelist[0] = hole_center.x;
    in.holelist[1] = hole_center.y;
///////////////////////////////////////////////////
    
    // Can be ignored safely
    in.numberofpointattributes = 0;
    in.pointattributelist = (REAL *) NULL;
    in.numberofregions = 0;
    in.regionlist = (REAL *) NULL;
    
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
    out.trianglearealist = (REAL *) NULL;
    out.holelist = (REAL *) NULL;
    out.regionlist = (REAL *) NULL;

////////////////////////////////////////////////////////////////
//TODO : test in order
//    triangulate("pcze", &in, &out, (struct triangulateio *) NULL);
//    triangulate("pze", &in, &out, (struct triangulateio *) NULL);
//    triangulate("pzea100.0", &in, &out, (struct triangulateio *) NULL);
    triangulate("pzea0.001q", &in, &out, (struct triangulateio *) NULL);
////////////////////////////////////////////////////////////////

    // Allow the use of program showme to visualize results
    saveToFiles(&out);
    
    // Build the mesh data-structure
    
    struct Mesh mesh;
    moveAndInit(&out, &mesh);

    // Compute gradient
    
////////////////////////////////////////////////////////////////
//TODO STEP 6


    REAL *f = malloc(mesh.n_points*sizeof(REAL));
    REAL *grad_f = malloc(2*mesh.n_points*sizeof(REAL));
    REAL c = 6.0; 

    for(int i = 0; i<mesh.n_points; ++i )
    {
        
	//TODO test case to check validity of gradient computation
        //  Initialize function values and closed-form gradients

	REAL yi = mesh.v_points[i].y;
	REAL xi = mesh.v_points[i].x;
	f[i] = sin(xi/c)*sin(yi/c);
        grad_f[2*i] = 1.0/c*cos(xi/c)*sin(yi/c);
        grad_f[2*i + 1] = 1.0/c*sin(xi/c)*cos(yi/c);

    }

    REAL *gradnum_f = malloc(2*mesh.n_points*sizeof(REAL));

	
    for(int i = 0; i<mesh.n_points; ++i )
    {
        //TODO Compute gradients
    
	int nn = mesh.nbNeighbors[i];

	REAL prod_x;
	REAL prod_y;


	REAL *opM = (REAL*) malloc(2*(nn+1)*sizeof(REAL));
	REAL *B = (REAL*) malloc(2*(nn+1)*sizeof(REAL));


	buildOpGrad(&mesh, i, opM, B);

	prod_x = opM[0] * f[i];
	prod_y = opM[nn+1] * f[i];

	for(int k = 0; k<nn; k++){
	    int j = mesh.neighbors[i*mesh.nmax + k];

	    prod_x += opM[k+1] * f[j];
	    prod_y += opM[nn+1 + k+1] * f[j];

	}

	gradnum_f[2*i] = prod_x;	
	gradnum_f[2*i + 1] = prod_y;
	
	printf("%g\n", gradnum_f[2*i]);
	printf("%g\n", gradnum_f[2*i + 1]);

        
    }

    REAL diff_x = 0;
    REAL diff_y = 0;

    for(int i = 0; i<mesh.n_points; ++i )
    {
        //TODO Compare closed-form and numerical gradients

    	diff_x += fabs(gradnum_f[2*i] - grad_f[2*i]);
    	diff_y += fabs(gradnum_f[2*i + 1] - grad_f[2*i + 1]);

    }

    diff_x /= mesh.n_points;
    diff_y /= mesh.n_points;

    printf("erreur_x = %g\n", diff_x);
    printf("erreur_y = %g\n", diff_y);

////////////////////////////////////////////////////////////////
 

    // Free all the memory
    free(in.pointlist);
    free(in.pointattributelist);
    free(in.pointmarkerlist);
    free(in.regionlist);
    free(mesh.v_points);
    free(mesh.v_triangles);
    free(mesh.v_edges);
    free(mesh.v_boundary_id);
    free(mesh.nbNeighbors);
    free(mesh.neighbors);
    free(mesh.barycoord);
    
    return 0;
}

