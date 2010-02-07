#pragma once

#ifdef __cplusplus
extern "c" {
#endif

#include "cv.h"
//#include <vector>

void naiveDistTransform(CvPoint * zeros,
						int numzeros,
						CvMat * dst);

#ifdef __cplusplus
}
#endif
