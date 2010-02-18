#include <iostream>
#include <string>
#include <cstdlib>

#include "EdgeDector.h"
#include "highgui.h"
#include "cv.h"

using namespace std;
using namespace cv;

int main (int argc, char * argv[])
{
	if (argc < 2) {
		cout << "Usage: main <image-file_name>\n";
		exit(0);
	}

	Mat edges;

	namedWindow("Image", 1);
	
	CannyDector(argv[1], edges, 400.0, 650.0, 5, true);

	imshow("Image", edges);
	string destImage = argv[1];
	destImage = "results/edges_" +
		destImage.substr(destImage.find_last_of("\\/")+1);
	cout << destImage << endl;
	imwrite(destImage, edges);

	waitKey(0);
}