#ifndef EDGEDECTOR_H
#define EDGEDECTOR_H

#include "cv.h"
#include <QImage>

class EdgeDector {

public:
	EdgeDector() {}
	EdgeDector(const cv::Mat & img) { this->img = img;}
	EdgeDector(const char * filename);

	void setImage(const cv::Mat & img) { this->img = img;}
	virtual void detectEdges(cv::Mat & edges) = 0;
	void detectEdges(const cv::Mat & img, cv::Mat & edges);
	void detectEdges(QImage & qEdges);
	
	virtual ~EdgeDector(){}

protected:
	cv::Mat img;
	cv::Mat _edges;
};

class CannyDector : public EdgeDector
{
public:
	CannyDector();
	CannyDector(const cv::Mat & img);
	CannyDector(const char * filename);
	CannyDector(const cv::Mat & img,
				cv::Mat & edges,
				double low,
				double high,
				double apertureSize,
				bool L2gradient);

	void detectEdges(cv::Mat & edges);

	// Set the parameter of Canny edge detection
	void setLowThresh(double low);
	void setHighThresh(double high);
	void setThresh(double low, double high);
	void setApertureSize(int apertureSize);
	void setL2gradient(bool L2gradient);
	void setParam(double low, double high, double apertureSize, bool L2gradient);
private:
	void init();

private:
	// Parameters for Canny edge detection
	double lowThresh; //Default 100 for canny
	double highThresh; // Default 150 for canny
	int apertureSize; // Default 3 for canny
	bool L2gradient; //Default false
};	
#endif
