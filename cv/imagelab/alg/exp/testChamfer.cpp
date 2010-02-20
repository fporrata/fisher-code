#include "alg.h"

#include "cv.h"
#include "util.h"
#include "highgui.h"

#include <iostream>

using namespace std;
using namespace cv;

int main (int argc, char * argv[]) {

	if (argc < 2) {
		cout << "Usage: main <image-file_name>\n";
		exit(0);
	}
	Mat img1 = imread(argv[1], 0);
	Mat src(img1.size(), CV_8UC1);
	img1.convertTo(src, CV_8UC1);

	//src = Mat::ones(img1.size(), CV_8UC1) * 255 - src;

	// Reverse the image to expose the zeros points
	reverseMat8UC1(src, src);


	Mat dest(src.size(), CV_32FC1);
	Mat labels(src.size(), CV_32SC1);
	
	distanceTransform(src, dest, CV_DIST_L2, CV_DIST_MASK_PRECISE);
	/*for (int y = 0; y < src.size().height; y++) {
		for (int x = 0; x < src.size().width; x++) {
			cout << (int)src.at<uchar>(x, y) << " ";
		}
		cout << endl;
	}*/
	for (int y = 0; y < src.size().height; y++) {
		for (int x = 0; x < src.size().width; x++) {
			cout << (float)dest.at<float>(x, y) << " ";
		}
		cout << endl;
	}
	mat2Scale(dest, dest, 0, 1);
	namedWindow("Image", 1);
	imshow("Image", dest);

	waitKey(0);

	return 0;
}
