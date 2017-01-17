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

#include <stdio.h>
#include <stdlib.h>

//#include "triangle.h"

#include "triangleIO.h"

//TODO:@todo : warning with 1 and 0 based indexing (z flag should be present to have 0)

void savePoly(struct triangulateio *io)
{
    char polyfilename[2048] = "out.1.poly";

    FILE *outfile;
    outfile = fopen(polyfilename, "w");
    if (outfile == (FILE *) NULL) {
        printf("  Error:  Cannot create file %s.\n", polyfilename);
        exit(1);
    }
    
    fprintf(outfile, "%d  %d  %d  %d\n",
            0, // vertices in separate node file
            2, // dimension
            io->numberofpointattributes,
            (io->pointmarkerlist==NULL)? 0 : 1);

    fprintf(outfile, "%d  %d\n",
            io->numberofsegments,
            (io->segmentmarkerlist==NULL)? 0 : 1);

//    fprintf(outfile, "Point to segment %p\n",io->segmentlist);
//    fprintf(outfile, "Point to edges %p\n",io->edgelist);

    for(int i = 0; i<io->numberofsegments; i++){
        if(io->segmentmarkerlist==NULL) {
            fprintf(outfile, "%4d    %4d  %4d\n",
                    i,
                    io->segmentlist[i*2+0],
                    io->segmentlist[i*2+1]);
        } else {
            fprintf(outfile, "%4d    %4d  %4d    %4d\n",
                    i,
                    io->segmentlist[i*2+0],
                    io->segmentlist[i*2+1],
                    io->segmentmarkerlist[i]);
        }
    }
    
    fprintf(outfile, "%d\n",
            io->numberofholes);

    if(io->numberofholes>0) {
        for(int i = 0; i<io->numberofholes; i++){
            fprintf(outfile, "%4d    %.17g  %.17g\n",
                    i,
                    io->holelist[i*2+0],
                    io->holelist[i*2+1]);
        }
    }
    
    fprintf(outfile, "\n");
    fclose(outfile);
}

void saveNode(struct triangulateio *io)
{
    char nodefilename[2048] = "out.1.node";

    FILE *outfile;
    outfile = fopen(nodefilename, "w");
    if (outfile == (FILE *) NULL) {
        printf("  Error:  Cannot create file %s.\n", nodefilename);
        exit(1);
    }

    fprintf(outfile, "%d  %d  %d  %d\n",
            io->numberofpoints,
            2, // number of dimension
            io->numberofpointattributes,
            (io->pointmarkerlist==NULL)? 0 : 1);

    for (int i = 0; i<io->numberofpoints; i++) {
        fprintf(outfile, "%4d    %.17g  %.17g", i,
                io->pointlist[i*2+0],
                io->pointlist[i*2+1]);

        if(io->numberofpointattributes>0) {
            for (int j = 0; j < io->numberofpointattributes; j++) {
                fprintf(outfile, "  %.17g", io->pointattributelist[i*io->numberofpointattributes + j]);
            }
        }

        if(io->pointmarkerlist!=NULL) {
            fprintf(outfile, "    %d", io->pointmarkerlist[i]);
        }

        fprintf(outfile,"\n");
    }
    

    fprintf(outfile, "\n");
    fclose(outfile);
}

void saveEle(struct triangulateio *io)
{
    char elefilename[2048] = "out.1.ele";

    FILE *outfile;
    outfile = fopen(elefilename, "w");
    if (outfile == (FILE *) NULL) {
        printf("  Error:  Cannot create file %s.\n", elefilename);
        exit(1);
    }
    
    fprintf(outfile, "%d  %d  %d\n", io->numberoftriangles, 3, io->numberoftriangleattributes);

    if(io->numberofcorners!=3) {
        printf("  Error:  number of corner not equal to 3 : %d\n", io->numberofcorners);
        exit(1);
    }
    for (int i = 0; i<io->numberoftriangles; i++) {
        fprintf(outfile, "%4d    %4d  %4d  %4d", i,
                io->trianglelist[i*3 + 0],
                io->trianglelist[i*3 + 1],
                io->trianglelist[i*3 + 2]);

        if (io->numberoftriangleattributes > 0) {
            for (int j = 0; j < io->numberoftriangleattributes; j++) {
                fprintf(outfile,"  %.6g", io->triangleattributelist[i * io->numberoftriangleattributes + j]);
            }
        }

        fprintf(outfile,"\n");
    }
    
    fprintf(outfile, "\n");
    fclose(outfile);
}

void saveEdge(struct triangulateio *io)
{
    char edgefilename[2048] = "out.1.edge";
    
    FILE *outfile;
    outfile = fopen(edgefilename, "w");
    if (outfile == (FILE *) NULL) {
        printf("  Error:  Cannot create file %s.\n", edgefilename);
        exit(1);
    }
    
    fprintf(outfile, "%d  %d\n",
            io->numberofedges,
            (io->edgemarkerlist==NULL)?0:1);
    
    for(int i = 0; i<io->numberofedges; ++i ) {
        if(io->edgemarkerlist==NULL){
            fprintf(outfile, "%4d   %d  %d\n", i,
                    io->edgelist[i*2 + 0],
                    io->edgelist[i*2 + 1]);
        } else {
            fprintf(outfile, "%4d   %d  %d  %d\n", i,
                    io->edgelist[i*2 + 0],
                    io->edgelist[i*2 + 1],
                    io->edgemarkerlist[i]);
        }
    }
    
    fprintf(outfile, "\n");
    fclose(outfile);
}

void saveToObj(struct triangulateio *io)
{
    char objfilename[2048] = "out.obj";
    
    FILE *outfile;
    outfile = fopen(objfilename, "w");
    if (outfile == (FILE *) NULL) {
        printf("  Error:  Cannot create file %s.\n", objfilename);
        exit(1);
    }
    
    fprintf(outfile, "# nb vertices = %d, nb faces = %d\n", io->numberofpoints, io->numberoftriangles);
    for (int i = 0; i<io->numberofpoints; i++) {
        fprintf(outfile, "v %.17g  %.17g  %.17g\n",
                io->pointlist[i*2+0],
                io->pointlist[i*2+1],
                0.0);
    }
    
    // WARNING : obj file indices start at 1
    for (int i = 0; i<io->numberoftriangles; i++) {
        fprintf(outfile, "f %4d  %4d  %4d\n",
                io->trianglelist[i*3 + 0]+1,
                io->trianglelist[i*3 + 1]+1,
                io->trianglelist[i*3 + 2]+1);
    }
    
    fprintf(outfile, "\n");
    fclose(outfile);
}

void saveToFiles(struct triangulateio *io)
{
    saveEle(io);
    saveNode(io);
    savePoly(io);
    saveEdge(io);
    
    saveToObj(io);
}


void report(struct triangulateio *io,
            int markers, int reporttriangles,
            int reportneighbors, int reportsegments,
            int reportedges, int reportnorms)
{
    int i, j;
    
    for (i = 0; i < io->numberofpoints; i++) {
        printf("Point %4d:", i);
        for (j = 0; j < 2; j++) {
            printf("  %.6g", io->pointlist[i * 2 + j]);
        }
        if (io->numberofpointattributes > 0) {
            printf("   attributes");
        }
        for (j = 0; j < io->numberofpointattributes; j++) {
            printf("  %.6g",
                   io->pointattributelist[i * io->numberofpointattributes + j]);
        }
        if (markers) {
            printf("   marker %d\n", io->pointmarkerlist[i]);
        } else {
            printf("\n");
        }
    }
    printf("\n");
    
    if (reporttriangles || reportneighbors) {
        for (i = 0; i < io->numberoftriangles; i++) {
            if (reporttriangles) {
                printf("Triangle %4d points:", i);
                for (j = 0; j < io->numberofcorners; j++) {
                    printf("  %4d", io->trianglelist[i * io->numberofcorners + j]);
                }
                if (io->numberoftriangleattributes > 0) {
                    printf("   attributes");
                }
                for (j = 0; j < io->numberoftriangleattributes; j++) {
                    printf("  %.6g", io->triangleattributelist[i *
                                                               io->numberoftriangleattributes + j]);
                }
                printf("\n");
            }
            if (reportneighbors) {
                printf("Triangle %4d neighbors:", i);
                for (j = 0; j < 3; j++) {
                    printf("  %4d", io->neighborlist[i * 3 + j]);
                }
                printf("\n");
            }
        }
        printf("\n");
    }
    
    if (reportsegments) {
        for (i = 0; i < io->numberofsegments; i++) {
            printf("Segment %4d points:", i);
            for (j = 0; j < 2; j++) {
                printf("  %4d", io->segmentlist[i * 2 + j]);
            }
            if (markers) {
                printf("   marker %d\n", io->segmentmarkerlist[i]);
            } else {
                printf("\n");
            }
        }
        printf("\n");
    }
    
    if (reportedges) {
        for (i = 0; i < io->numberofedges; i++) {
            printf("Edge %4d points:", i);
            for (j = 0; j < 2; j++) {
                printf("  %4d", io->edgelist[i * 2 + j]);
            }
            if (reportnorms && (io->edgelist[i * 2 + 1] == -1)) {
                for (j = 0; j < 2; j++) {
                    printf("  %.6g", io->normlist[i * 2 + j]);
                }
            }
            if (markers) {
                printf("   marker %d\n", io->edgemarkerlist[i]);
            } else {
                printf("\n");
            }
        }
        printf("\n");
    }
}

