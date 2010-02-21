#include "alg.h"
#include "util.h"

#include <math.h>
#include <iostream>

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

void naiveDistTransform(std::vector<cv::Point2i> zeros, cv::Mat & dst)
{
	cv::Size size = dst.size();
	double distance;
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			dst.at<double>(x, y) = sq_dist(cv::Point2i(x, y), zeros[0]);
			for (int i = 1; i < zeros.size(); i++) {
				distance = sq_dist(cv::Point2i(x, y), zeros[i]);
				if (distance < dst.at<double>(x, y)) dst.at<double>(x, y) = distance;
			}
			dst.at<double>(x, y) = distance;
		}
	}
}

void getZeros8UC1(const cv::Mat & src, std::vector<cv::Point2i> & zeros)
{	
	assert(src.type() == CV_8UC1);
	for (int y = 0; y < src.size().height; y++) {
		for (int x = 0; x < src.size().width; x++) {
			if (src.at<uchar>(y, x) == 0) {
				zeros.push_back(cv::Point2i(x, y));
			}
		}
	}
}

double sumPoints8UC1(const cv::Mat & src, const std::vector<cv::Point2i> & points)
{
	assert(src.type() == CV_8UC1);
	double sum = 0;
	for (std::vector<cv::Point2i>::const_iterator it = points.begin();
			it < points.end(); it++) {
		sum += src.at<uchar>(*it);
	}

	return sum;
}

double sumPoints32FC1(const cv::Mat & src, const std::vector<cv::Point2i> & points)
{
	assert(src.type() == CV_32F);
	double sum = 0;
	for (std::vector<cv::Point2i>::const_iterator it = points.begin();
			it < points.end(); it++) {
		sum += src.at<float>(*it);
	}

	return sum;
}

double sumPoints(const cv::Mat * src, const std::vector<cv::Point2i> & points)
{}

double chamferMatching(const cv::Mat & src, const cv::Mat & target, cv::Point2i & pos)
{
	std::vector<cv::Point2i> zeros;
	getZeros8UC1(target, zeros);
	std::cout << "Zeros of the target image are ready.\n";
	std::cout << "There are " << zeros.size() << " zeros.\n";
	//std::cout << "Sum of the zeros is " << sumPoints8UC1(target, zeros) << std::endl;
	return chamferMatching(src, zeros, target.size(), pos);
}

double chamferMatchingScore(const cv::Mat & srcDist,
							const cv::Mat & target,
							cv::Point2i pos)
{
	std::vector<cv::Point2i> zeros;
	getZeros8UC1(target, zeros);
	std::cout << "Zeros of the target image are ready.\n";
	std::cout << "There are " << zeros.size() << " zeros.\n";
	return chamferMatchingScore(srcDist, zeros, pos);
}

double chamferMatchingScore(const cv::Mat & srcDist,
							const std::vector<cv::Point2i> & zeros,
							cv::Point2i pos)
{
	assert(srcDist.type() == CV_32F);
	double score = 0;
	for (std::vector<cv::Point2i>::const_iterator it = zeros.begin();
			it < zeros.end(); it++) {
		score += srcDist.at<float>(pos+*it);
	}
	//score = sumPoints(srcDist, zeros);

	return score;
}

double chamferMatching(const cv::Mat & src, 
					   const std::vector<cv::Point2i> & zeros, 
					   cv::Size targetSize,
					   cv::Point2i & pos)
{
	double bestscore = INFINITY;
	pos = cv::Point2i(0, 0);
	for (int y = 0; y <= src.size().height - targetSize.height; y++) {
		for (int x = 0; x <= src.size().width - targetSize.width; x++) {
			double score = chamferMatchingScore(src, zeros, cv::Point2i(x, y));
			//std::cout << "Score is " << score << " when x: " << x << " y: " << y << std::endl;
			if (score < bestscore) {
				bestscore = score;
				pos = cv::Point2i(x, y);
			}
		}
	}
	return bestscore;
}
			
