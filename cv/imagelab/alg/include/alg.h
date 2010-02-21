#pragma once

/*#ifdef __cplusplus
extern "c" {
#endif*/

#include "cv.h"
#include <vector>

void naiveDistTransform(CvPoint * zeros,
						int numzeros,
						CvMat * dst);

void naiveDistTransform(std::vector<cv::Point2i> zeros, cv::Mat & dst);

/*
   src should be of type CV_32FC1
*/
void getZeros(const cv::Mat & src, std::vector<cv::Point2i> & zeros);


/*
 * src should be of type CV_32FC1
 * target would be of type CV_8UC1
 * return the score
 */
double chamferMatching(const cv::Mat & src, const cv::Mat & target, cv::Point2i & pos);
double chamferMatching(const cv::Mat & src, 
					   const std::vector<cv::Point2i> & zeros, 
					   cv::Size targetSize,
					   cv::Point2i & pos);
double chamferMatchingScore(const cv::Mat & srcDist,
					   		const std::vector<cv::Point2i> & zeros,
					   		cv::Point2i pos);
double chamferMatchingScore(const cv::Mat & srcDist,
							const cv::Mat & target,
							cv::Point2i pos);

/*#ifdef __cplusplus
}
#endif*/
