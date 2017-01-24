#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <assert.h>

#include "mesh.h"

//#define freq 44100
#define freq 44100 //352800
#define duration 1

struct Mesh *mesh;

REAL *uc, *up, *us;
REAL *d4uc, *d4up;

// Temporaries used to avoid multiple dynamic allocations
REAL *d_tmp, *d4x, *d4y, *d2xy;


void computeDeriv(int i, int off, REAL *f)
{
    unsigned int maxMatrixSize = 2*(mes->nmax+1);
    unsigned int mOff = i*maxMatrixSize+off;

    int nn = mesh ->nbNeighbors[i];

    REAL res = mesh->gradOp[mOff+0] * f[i];
    for(int k=0; k<nn; ++k) {
        res += mesh->gradOp[mOff+k+1]*f[mesh->neighbors[i*mesh->nmax+1]];
    }

    return res;
}

REAL averageFromNeighbors(int i, REAL *f)
{
    unsigned int maxMatrixSize = (mesh->nmax);
    int nn = mesh->nbNeighbors[i];
    unsigned int mOff = i*maxMatrixSize;

    REAL res = 0.0;
    for(int k=0; k<nn; ++k) {
        res +=mesh->barycoord[mOff+k] * f[mesh->neighbors[i*mesh->nmas+k];
    }
}

void computeD4(REAL a, REAL b, REAL c, REAL* u, REAL* d4u)
{

}

void next(REAL A, REAL B, REAL C, REAL D, REAL a, REALb, REAL c)
{
    int i;
    for (i = 1; i<mesh->n_points; ++i) {
        if(mesh->v_boundary_id[i] !=2 && mesh->v_boundary_id[i] != -2) {
            us[i] = A + uc[i]*(2-R*dt) + up[i]*(D-1) + B*d4uc[i] + C*d4up[i];
        }

    }

    REAL *up_backup = up;
    up = uc;
    uc = us;
    us = up_backup; //on reecrit sur ces valeurs qui ne nous servent plus

    up_backup = d4up;
    d4up = d4uc;
    d4uc = up_backup; //on reecrit sur des valeurs qui ne nous servent plus
    computeD4(a, b, c, uc, d4uc)
}


int main()
{
    // Read binary mesh
    
    mesh = (struct Mesh*)malloc(sizeof(struct Mesh));
    loadBinaryMesh(mesh, "binaryMesh.bin");
    
    REAL dt = 1.0f/freq;
    
    REAL son[freq*duration];
    
    REAL delta = 0.0029; // m epaisseur table
    REAL rho = 350.0; // kg.m-3
    //    REAL eta = 0.005; // s
    REAL eta = 0.0; // s
    REAL R = 7.0; // s-1
    
    REAL d2 = delta*delta / (12.0*rho);
    REAL A = dt*dt / (delta * rho);
    REAL B = -(eta+dt) * dt * d2; //TODO : sign might be wrong
    REAL C =   eta   * dt * d2; //TODO : sign might be wrong
    //        REAL B = (eta+dt) * dt * d2; //TODO : sign might be wrong
    //        REAL C = -  eta   * dt * d2; //TODO : sign might be wrong
    REAL D = R * dt;
    
    REAL nu_xy = 0.3;
    REAL nu_yx = 0.3; //symetric ???
    REAL E_x = 9925.0 * 1.0e6; //Pa
    REAL E_y = 852.0 * 1.0e6; //Pa
    REAL G_xy = 600.0 * 1.0e6; //Pa = kg /(m.s^2) = N / m^2
    
    REAL a = E_x / (1.0-nu_xy*nu_yx) ;
    REAL b = 4.0 * G_xy - (E_y*nu_xy + E_x*nu_yx)/ (1.0-nu_xy*nu_yx);
    REAL c = E_y / (1.0-nu_xy*nu_yx);

    up = calloc(mesh->n_points, sizeof(REAL));
    uc = calloc(mesh->n_points, sizeof(REAL));
    us = calloc(mesh->n_points, sizeof(REAL));
    d4uc = calloc(mesh->n_points, sizeof(REAL));
    d4up = calloc(mesh->n_points, sizeof(REAL));


}
