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
	/*if (argc != 3) {
		cout << "Usage: main <src-file_name> <target-file_name>\n";
		exit(0);
	}*/
	int numImages = argc - 1;
	vector<string> imageNames;
	for (int i = 0; i < numImages; i++) {
		imageNames.push_back(argv[i+1]);
	}

	//int width_arr[] = {500, 1000, 1500, 2000, 2500};
	int width_arr[] = {0};
	vector<int> width(width_arr, width_arr + sizeof(width_arr) / sizeof(int));;
	

	for (int i = 0; i < numImages; i++) {
		cout << "Processing image: " << imageNames[i] << endl;
		Mat image = imread(imageNames[i], 0);
		// different scale
		for (int j = 0; j < width.size(); j++) {

			//Resize the original image
			//Mat curImage(Size(width[j], image.size().height * width[j] / image.size().width), 
					//CV_MAKETYPE(image.depth(), image.channels()));
					//CV_8UC1);
			//resize(image, curImage, curImage.size());
			Mat dest(image.size(), CV_8UC1);

			//namedWindow("Image", 1);

			//CannyDector(curImage, dest, 400.0, 650.0, 5, true);
			CannyDector(image, dest, 120.0, 150.0, 3, false);

			Mat edges(dest.size(), CV_8UC1);
			dest.convertTo(edges, CV_8UC1);
			reverseMat8UC1(edges, edges);

			// Store the processed edge images
			//imshow("Image", edges);
			//string destImage = argv[1];
			stringstream destImageName;
			destImageName << "edge_data/edges_";
			destImageName << width[j] << "_";
			destImageName << imageNames[i].substr(imageNames[i].find_last_of("\\/")+1);
			//cout << destImage << endl;
			imwrite(destImageName.str(), edges);

			// Release memory
			//curImage.release();
			dest.release();
			edges.release();
		}
		image.release();
	}

	//waitKey(0);
}
