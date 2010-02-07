#include "_alg.h"

#include <math.h>

void naiveDistTransform(CvPoint * zeros,
						int numzeros,
						CvMat * dst)
{
	//assert(dst != 0 && src->rows != dst->rows &&
	//		src->cols != dst->cols);

	int x, y, k;
	double * dbase = dst->data.db;
	double dist;

	for (y = 0; y < dst->rows; y++) {
		for (x = 0; x < dst->cols; x++) {
			dbase[x] = INFINITY;
			for (k = 0; k < numzeros; k++) {
				dist = sqrt((x - zeros[k].x) * (x - zeros[k].x) +
							(y - zeros[k].y) * (y - zeros[k].y));
				if (dist < *dbase) dbase[x] = dist;
			}
		}
		dbase += dst->step;
	}
}
