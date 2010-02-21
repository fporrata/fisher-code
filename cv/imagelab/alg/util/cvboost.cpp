#include "util.h"

#include <algorithm>
#include <iostream>

using namespace std;

void overlay(const cv::Mat & src1, const cv::Mat & src2,
			cv::Mat & dest, cv::Point2i pos)
{
	int cn = std::max(src1.channels(), src2.channels());
	int type = std::max(src1.type(), src2.type());
	cv::Size size1 = src1.size();
	cv::Size size2 = src2.size();
	dest = cv::Mat(std::max(size1.height, size2.height + pos.y),
			   std::max(size1.width, size2.width + pos.x),
			   type);
	cv::Mat tmp_src1 = src1.reshape(cn);
	cv::Mat tmp_src2 = src2.reshape(cn);

	cv::Mat sub1 = dest(cv::Rect(0, 0, size1.width, size1.height));
	tmp_src1.copyTo(sub1);
	cv::Mat sub2 = dest(cv::Rect(pos.x, pos.y, size2.width, size2.height));
	tmp_src2.copyTo(sub2);
	/*for (int i = 0; i < size1.height; i++) {
		for (int j = 0; j < size1.width; j++) {
			dest.at<double>(i, j) = tmp_src1.at<int>(i, j);
			//for (int c = 0; c < cn; c++) {
			//	dest(i,j)[c] = tmp_src1(i, j)[c];
			//}
		}
	}*/

	/*for (int i = 0; i < size2.height; i++) {
		for (int j = 0; j < size2.width; j++) {
			dest.at<double>(i + pos.y, j + pos.x) = tmp_src2.at<double>(i, j);
		}
	}*/
}

double sq_dist(cv::Point2i p1, cv::Point2i p2) 
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

double dist(cv::Point2i p1, cv::Point2i p2)
{
	return sqrt(sq_dist(p1, p2));
}


float maxMat32FC1(const cv::Mat & src)
{
	float maxValue = src.at<float>(0, 0);
	for (int i = 0; i < src.size().height; i++) {
		for (int j = 0; j < src.size().width; j++) {
			maxValue = max(maxValue, src.at<float>(i, j));
		}
	}

	return maxValue;
}

float minMat32FC1(const cv::Mat & src)
{
	float minValue = src.at<float>(0, 0);
	for (int i = 0; i < src.size().height; i++) {
		for (int j = 0; j < src.size().width; j++) {
			minValue = min(minValue, src.at<float>(i, j));
		}
	}

	return minValue;
}


void mat2Scale(const cv::Mat & src, cv::Mat & dest, double min, double max)
{
	cv::Mat src_copy(src.size(), CV_32FC1);
	src.convertTo(src_copy, CV_32FC1);
	//dest = cv::Mat(src.size(), CV_32FC1);

	float maxValue = maxMat32FC1(src_copy);
	float minValue = minMat32FC1(src_copy);
	
	for (int i = 0; i < src.size().height; i++) {
		for (int j = 0; j < src.size().width; j++) {
			dest.at<float>(i, j) = 
				(src_copy.at<float>(i, j) - minValue) * (max - min) / (maxValue - minValue) + min;
		}
	}
}

void reverseMat8UC1(const cv::Mat & src, cv::Mat & dest)
{
	for (int i = 0; i < src.size().height; i++) {
		for (int j = 0; j < src.size().width; j++) {
			dest.at<uchar>(i, j) = (src.at<uchar>(i, j) > 0)? 0 : 255;
		}
	}
}

void toWB8UC1(const cv::Mat & src, cv::Mat & dest)
{
	//dest = cv::Mat(src.size(), CV_8UC1);
	for (int y = 0; y < src.size().height; y++) {
		for (int x = 0; x < src.size().width; x++) {
			dest.at<uchar>(y, x) = (src.at<uchar>(y, x) == 0)? 0 : 255;
		}
	}
}
