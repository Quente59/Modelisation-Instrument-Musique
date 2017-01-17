
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

void savePoly(struct triangulateio *io);
void saveNode(struct triangulateio *io);
void saveEle(struct triangulateio *io);
void saveEdge(struct triangulateio *io);

void saveToObj(struct triangulateio *io);

void saveToFiles(struct triangulateio *io);


void report(struct triangulateio *io,
            int markers, int reporttriangles,
            int reportneighbors, int reportsegments,
            int reportedges, int reportnorms);
