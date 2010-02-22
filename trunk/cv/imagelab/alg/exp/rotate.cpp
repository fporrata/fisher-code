// Experiment on the rotation matrix

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

#include "class.h"
#include "highgui.h"
#include "cv.h"
#include "util.h"

using namespace std;
using namespace cv;

int main (int argc, char * argv[])
{
	if (argc != 2) {
		cout << "Usage: main <image_to_rotate>\n";
		exit(0);
	}

	image = imread(argv[1]);
	double angle_arr[] = {0, 90, 180, 270};
	vector<double> angles(angle_arr, angle_arr+sizeof(angle_arr)/sizeof(double));

	for (int i = 0; i < angles.size(); i++) {
		Mat rotMat = getRotationMatrix2D(Point2f(image.size().width / 2.0,
										 		 image.size().height / 2.0),
										 angles[i], 1);
		Mat dest;
		
