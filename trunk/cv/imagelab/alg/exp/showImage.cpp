#include <iostream>
#include <string>
#include "cv.h"
#include "highgui.h"

using namespace std;
using namespace cv;

int main (int argc, char * argv[])
{
	if (argc < 2) {
		cout << "Usage: main <image-file_name>\n";
	}

	Mat image = imread(argv[1]);

	namedWindow("Image", 1);
	
	imshow("Image", image);
	
	waitKey(0);
}
