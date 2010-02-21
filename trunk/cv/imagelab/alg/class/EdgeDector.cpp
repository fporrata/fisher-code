#include "class.h"
#include "highgui.h"

void EdgeDector::detectEdges(const cv::Mat & img, cv::Mat & edges) {
	this->img = img;
	_detectEdges(edges);
}

void EdgeDector::detectEdges(cv::Mat & edges)
{
	_detectEdges(edges);
}

/*void EdgeDector::detectEdges(QImage & qEdges)
{
	detectEdges(_edges);
	qEdges = QImage(_edges.data, _edges.cols, _edges.rows, QImage::Format_RGB888);
	}*/

EdgeDector::EdgeDector(const char * filename)
{
	img = cv::imread(filename);
}

CannyDector::CannyDector() : EdgeDector(img)
{
	init();
}

CannyDector::CannyDector(const cv::Mat & img) : EdgeDector(img)
{
	init();
}

CannyDector::CannyDector(const char * filename)
{
	img = cv::imread(filename, 0);
	init();
}

CannyDector::CannyDector(const cv::Mat & img,
						 cv::Mat & edges,
				 	 	 double low,
						 double high,
						 double apertureSize,
						 bool L2gradient) : EdgeDector(img)
{
	setParam(low, high, apertureSize, L2gradient);
	//EdgeDector::detectEdges(img, edges);
	detectEdges(edges);
}

CannyDector::CannyDector(const char * filename,
						 cv::Mat & edges,
						 double low,
						 double high,
						 double apertureSize,
						 bool L2gradient)
{
	img = cv::imread(filename, 0);
	setParam(low, high, apertureSize, L2gradient);
	detectEdges(edges);
}
	
void CannyDector::init()
{
			lowThresh = 100;
			highThresh = 150;
			apertureSize = 3;
			L2gradient = false;
}

void CannyDector::_detectEdges(cv::Mat & edges)
{
	cv::Canny(img, edges, lowThresh, highThresh, apertureSize, L2gradient);
}

void CannyDector::setLowThresh(double low)
{
	lowThresh = low;
}

void CannyDector::setHighThresh(double high)
{
	highThresh = high;
}

void CannyDector::setApertureSize(int apertureSize)
{
	this->apertureSize = apertureSize;
}

void CannyDector::setL2gradient(bool L2gradient)
{
	this->L2gradient = L2gradient;
}

void CannyDector::setThresh(double low, double high)
{
	lowThresh =low;
	highThresh = high;
}

void CannyDector::setParam(double low, double high, double apertureSize, bool L2gradient)
{
	setThresh(low, high);
	this->apertureSize = apertureSize;
	this->L2gradient = L2gradient;
}
