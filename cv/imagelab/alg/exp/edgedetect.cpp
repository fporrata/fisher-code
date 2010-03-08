#include <iostream>
#include <string>
#include <cstdlib>

#include "class.h"
#include "highgui.h"
#include "cv.h"
#include "util.h"

using namespace std;
using namespace cv;

int main (int argc, char * argv[])
{
	if (argc != 3) {
		cout << "Usage: main <src-file_name> <target-file_name>\n";
		exit(0);
	}

	Mat dest;

	namedWindow("Image", 1);
	
	CannyDector(argv[1], dest, 500.0, 650.0, 5, true);

	Mat edges(dest.size(), CV_8UC1);
	dest.convertTo(edges, CV_8UC1);
	reverseMat8UC1(edges, edges);

	imshow("Image", edges);
	//string destImage = argv[1];
	//destImage = "results/edges_" +
	//	destImage.substr(destImage.find_last_of("\\/")+1);
	//cout << destImage << endl;
	imwrite(argv[2], edges);

	waitKey(0);
}
