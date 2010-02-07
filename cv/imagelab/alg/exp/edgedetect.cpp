#include <iostream>
#include <cstdlib>

#include "EdgeDector.h"
#include "highgui.h"

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
	
	CannyDector("yufu.jpg", edges, 400.0, 650.0, 5, true);

	imshow("Image", edges);
	imwrite("yufu_edges.jpg", edges);

	waitKey(0);
}
