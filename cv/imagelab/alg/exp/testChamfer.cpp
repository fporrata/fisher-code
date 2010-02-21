#include "alg.h"

#include "cv.h"
#include "util.h"
#include "highgui.h"

#include <iostream>

using namespace std;
using namespace cv;

int main (int argc, char * argv[]) {

	/*if (argc < 2) {
		cout << "Usage: main <image-file_name>\n";
		exit(0);
	}*/
	//Mat img1 = imread(argv[1], 0);

	cout << "===== Prepare the images =====\n";
	Mat img1 = imread("images/stapler_src_edges.jpg", 0);
	Mat img2 = imread("images/stapler_target_edges1.jpg", 0);
	Mat src(img1.size(), CV_8UC1);
	Mat target(img2.size(), CV_8UC1);
	img1.convertTo(src, CV_8UC1);
	img2.convertTo(target, CV_8UC1);
	
	toWB8UC1(src, src);
	toWB8UC1(target, target);

	//src = Mat::ones(img1.size(), CV_8UC1) * 255 - src;

	// Reverse the image to expose the zeros points
	//reverseMat8UC1(src, src);
	//reverseMat8UC1(target, target);


	Mat srcDist(src.size(), CV_32FC1);
	Mat labels(src.size(), CV_32SC1);
	
	cout << "===== Distance transform =====\n";
	distanceTransform(src, srcDist, CV_DIST_L1, 3); //CV_DIST_MASK_PRECISE);
	Mat srcDist_rev(srcDist.size(), CV_8UC1);
	srcDist.convertTo(srcDist_rev, CV_8UC1);
	toWB8UC1(srcDist_rev, srcDist_rev);
	/*for (int y = 0; y < src.size().height; y++) {
		for (int x = 0; x < src.size().width; x++) {
			cout << (int)src.at<uchar>(x, y) << " ";
		}
		cout << endl;
	}*/
	/*for (int y = 0; y < src.size().height; y++) {
		for (int x = 0; x < src.size().width; x++) {
			cout << (float)dest.at<float>(x, y) << " ";
		}
		cout << endl;
	}*/
	//mat2Scale(srcDist, srcDist, 0, 1);
	//Mat tmp = abs(srcDist_rev - target);
	/*namedWindow("srcDist_rev", 1);
	imshow("srcDist_rev", srcDist);
	waitKey();
*/

	// chamfer matching
	cout << "===== Chamfer matching =====\n";
	Point2i pos(400, 709);
	cout << "Target image size: " << target.size().height << " " << target.size().width << endl;
	cout << "srcDist_rev size: " << srcDist_rev.size().height << " " << srcDist_rev.size().width << endl;

	//double score = chamferMatchingScore(srcDist, target, pos);
	double score = chamferMatching(srcDist, target, pos);
	cout << "Score is " << score << endl;
	cout << "Position is " << pos.x << " " << pos.y << endl;
	Mat syn_image;
	// To show the target in the synthesized image
	Mat target_gray = target / 2;
	overlay(src, target_gray, syn_image, pos);

	namedWindow("Image", 1);
	imshow("Image", syn_image);

	waitKey(0);

	return 0;
}
