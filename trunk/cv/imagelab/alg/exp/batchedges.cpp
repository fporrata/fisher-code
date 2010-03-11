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

	//int width_arr[] = {500, 800, 1000};
	int width_arr[] = {0};
	vector<int> width(width_arr, width_arr + sizeof(width_arr) / sizeof(int));
	

	for (int i = 0; i < numImages; i++) {
		cout << "Processing image: " << imageNames[i] << endl;
		Mat image = imread(imageNames[i], 0);
		// different scale
		for (int j = 0; j < width.size(); j++) {

			//Resize the original image
			Mat curImage;
			
			if (width[j] != 0) {
				curImage = Mat(Size(width[j], image.size().height * width[j] / image.size().width), 
					CV_MAKETYPE(image.depth(), image.channels()));
				resize(image, curImage, curImage.size());
			} else {
				//curImage = Mat(image.size(), image.type());
				curImage = image.clone();
			}
					//CV_8UC1);
			Mat dest(curImage.size(), CV_8UC1);

			//namedWindow("Image", 1);

			//CannyDector(curImage, dest, 500.0, 650.0, 5, true);
			CannyDector(curImage, dest, 100.0, 150.0, 3, true);
			//CannyDector(image, dest, 120.0, 150.0, 3, false);

			Mat edges(dest.size(), CV_8UC1);
			dest.convertTo(edges, CV_8UC1);
			reverseMat8UC1(edges, edges);

			// Store the processed edge images
			//imshow("Image", edges);
			//string destImage = argv[1];
			stringstream destImageName;
			destImageName << "edge_data/edges_";
			if (width[j] != 0)
				destImageName << width[j] << "_";
			else
				destImageName << curImage.size().width << "_";
			destImageName << imageNames[i].substr(imageNames[i].find_last_of("\\/")+1);
			//cout << destImage << endl;
			imwrite(destImageName.str(), edges);

			// Release memory
			curImage.release();
			dest.release();
			edges.release();
		}
		image.release();
	}

	//waitKey(0);
}
