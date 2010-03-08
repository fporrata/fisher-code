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

	Mat image = imread(argv[1], 0);
	toWB8UC1(image, image);
	reverseMat8UC1(image, image);

	// Initialize the angles
	int numRot = 60;
	vector<float> angles(numRot);
	for (int i = 0; i < numRot; i++) {
		angles[i] = 360.0 * i / numRot;
	}

	namedWindow("rotate", CV_WINDOW_AUTOSIZE);
	for (int i = 0; i < angles.size(); i++) {
		Mat rotMat = getRotationMatrix2D(Point2f(image.size().width / 2.0,
										 		 image.size().height / 2.0),
										 angles[i], 1);
		cout << "rotMat size: " << rotMat.size().width << " "
			 << rotMat.size().height << endl;
		cout << "src channels: " << image.channels() << endl;

		int side = sqrt(image.size().width * image.size().width + 
						image.size().height * image.size().height);
		//Mat dest(side, side, image.type());
		//warpAffine(image, dest, rotMat, Size2i(side, side));
		Mat dest;
		rotateImage(image, dest, angles[i]);

		cout << "dest channels: " << dest.channels() << endl;
		
		toWB8UC1(dest, dest);
		reverseMat8UC1(dest, dest);

		imshow("rotate", dest);
		waitKey();


		dest.release();
		rotMat.release();
	}
	image.release();
}
		
