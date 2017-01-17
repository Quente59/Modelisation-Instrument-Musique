
#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <assert.h>

#include "mesh.h"

/*------------------------------------------------------------------------*/

void matrixMult(REAL *A, REAL *B, REAL *R, int n_row, int n_col, int n) {
			
////////////////////////////////////////////////////////////////
//TODO : STEP 7
//TODO implement matrix multiplication here
// you can change the signature of the function if you find it useful=
////////////////////////////////////////////////////////////////
	
	for (int i=0; i<n_row; i++){
		for (int j = 0; j<n_col; j++){
			REAL res = 0.0;
			for (int k = 0; k<n; k++){
				res += A[i*n + k]*B[k*n_col + j];
			}
			R[i*n_col + j] = res;
		}
	}

}

void buildOpGrad(struct Mesh *mesh, int i, REAL *opM, REAL *B)
{
////////////////////////////////////////////////////////////////
//TODO : STEP 8
//TODO compute matrix opM
////////////////////////////////////////////////////////////////
	REAL xO = mesh->v_points[i].x;
	REAL yO = mesh->v_points[i].y;

	int nn = mesh->nbNeighbors[i];


	REAL Mxx = 0.0;
	REAL Mxy = 0.0;
	REAL Myy = 0.0;

	REAL Bx = 0.0;
	REAL By = 0.0;

	for (int k=0; k<nn; k++){
		int j = mesh->neighbors[i*mesh->nmax + k];
	
		REAL yj = mesh->v_points[j].y;
		REAL xj = mesh->v_points[j].x;
		REAL wj = mesh->barycoord[i*mesh->nmax + k];

		Mxx += wj*wj*(xj - xO)*(xj - xO);
		Mxy += wj*wj*(xj - xO)*(yj - yO);
		Myy += wj*wj*(yj - yO)*(yj - yO);

		Bx += wj*wj*(xj - xO);
		B[k+1] = wj*wj*(xj - xO);
		By += wj*wj*(yj - yO);
		B[nn+1 + k+1] = wj*wj*(yj - yO);

	}

	REAL coef = 1.0/(Mxx*Myy-Mxy*Mxy);

	REAL invM[4];

	invM[0] = coef*Myy;
	invM[1] = - Mxy*coef;
	invM[2] = - Mxy*coef;
	invM[3] = Mxx*coef;

	B[0] = -Bx; //CZ add minus
	B[nn+1] = -By; //CZ ...



	matrixMult(&invM, B, opM, 2, nn+1, 2);



}

/*------------------------------------------------------------------------*/

void moveAndInit(struct triangulateio *in, struct Mesh *mesh)
{
    assert(in->numberofcorners==3);

    mesh->n_points = in->numberofpoints;
    mesh->n_triangles = in->numberoftriangles;
    mesh->n_edges = in->numberofedges;

    mesh->v_points = (struct vec2*)in->pointlist;
    in->pointlist = NULL;
    
    mesh->v_triangles = (struct triangle*)in->trianglelist;
    in->trianglelist = NULL;

    mesh->v_edges = (struct edge*)in->edgelist;
    in->edgelist = NULL;

    mesh->v_boundary_id = in->pointmarkerlist;
    in->pointmarkerlist = NULL;
    
    // Initialize adjacency between vertices
    
    mesh->nbNeighbors = (int *) calloc(mesh->n_points,sizeof(int));
    
    // first we need to find the maximal number of neighbors per vertex
    mesh->nmax = 0;
    for(int i = 0; i<mesh->n_edges; ++i ) {
        mesh->nbNeighbors[ mesh->v_edges[i].v_id[0] ] += 1;
        mesh->nbNeighbors[ mesh->v_edges[i].v_id[1] ] += 1;
    }
    for(int i = 0; i<mesh->n_points; ++i ) {
        if(mesh->nbNeighbors[i]>mesh->nmax)
            mesh->nmax = mesh->nbNeighbors[i];
        mesh->nbNeighbors[i] = 0; // we need it to be zero for init of next step
    }
    printf("Maximum number of neighbors per Vertex : %d\n", mesh->nmax);
    
    mesh->neighbors = (int *) malloc(mesh->nmax * mesh->n_points*sizeof(int));
    
    for(int i = 0; i<mesh->n_edges; ++i ) {
        int k = mesh->v_edges[i].v_id[0];
        int l = mesh->v_edges[i].v_id[1];
        
        mesh->neighbors[k*mesh->nmax + mesh->nbNeighbors[k]] = l;
        mesh->nbNeighbors[k] += 1;
        
        mesh->neighbors[l*mesh->nmax + mesh->nbNeighbors[l]] = k;
        mesh->nbNeighbors[l] += 1;
    }
    
    mesh->barycoord = NULL;
    
    // Initialize the generalized barycentric coordinate
    computeMeanValueCoordinates(mesh);
}

/*------------------------------------------------------------------------*/

void computeMeanValueCoordinates(struct Mesh *mesh)
{
    if(mesh->barycoord!=NULL) {
        free(mesh->barycoord);
    }
    
    mesh->barycoord = (REAL *) calloc(mesh->nmax*mesh->n_points,sizeof(REAL));
    
////////////////////////////////////////////////////////////////
//TODO : STEP 9
//TODO : shoud be replaced by computation of generalized barycentric coordinates
    for(int i = 0; i<mesh->n_points; ++i ) {
        REAL v = 1.0/((REAL)mesh->nbNeighbors[i]);
        for (int j = 0; j<mesh->nbNeighbors[i]; ++j) {
            mesh->barycoord[i*mesh->nmax + j] = v;
        }
    }
////////////////////////////////////////////////////////////////
    
    for(int i = 0; i<mesh->n_points; ++i ) {
        REAL sum = 0.0;
        for (int j = 0; j<mesh->nbNeighbors[i];++j) {
            sum+= mesh->barycoord[i*mesh->nmax+j];
        }
        printf("%f",sum);
        printf("\n");
    }

    // Affiche les coordonnées de tout les vertex relatif à leur voisin
    
    for(int i = 0; i<mesh->n_points; ++i ) {
        if(mesh->v_boundary_id[i]==0)
        for (int j = 0; j<mesh->nbNeighbors[i];++j) {
            printf("%g ",mesh->barycoord[i*mesh->nmax+j]);
        }
        printf("\n");
    }
    
    // Simple test : compare the vertex with the position obtained from barycentric coordinates
    
    for(int i = 0; i<mesh->n_points; ++i ) {
        if(mesh->v_boundary_id[i]==0) {
            REAL x = 0.0;
            REAL y = 0.0;
            for(int k=0; k<mesh->nbNeighbors[i]; ++k) {
                x += mesh->v_points[mesh->neighbors[i*mesh->nmax+k]].x
                * mesh->barycoord[i*mesh->nmax+k];
                y += mesh->v_points[mesh->neighbors[i*mesh->nmax+k]].y
                * mesh->barycoord[i*mesh->nmax+k];
            }
            printf("Xerr : %g", fabs(mesh->v_points[i].x-x));
            printf("    Yerr : %g\n", fabs(mesh->v_points[i].y-y));
        }
    }
}

/*------------------------------------------------------------------------*/

