#ifndef __RectangleFinder__
#define __RectangleFinder__

#include "Logwrapper.h"
#include "opencv2/opencv.hpp"
#include <vector>

class RectangleFinder {
public:
	RectangleFinder();
	void detectRectangles(cv::Mat& image);

private:

	void init();
	void findSquares(const cv::Mat& image, std::vector<std::vector<cv::Point> >& squares);
	void drawSquares(cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares);

	void filterRectangles(std::vector<std::vector<cv::Point> > contours, std::vector<std::vector<cv::Point> >& squares);
	double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	void gendiamond(cv::Mat& dst, std::vector<int>& list);

	cv::Mat diamond;
	cv::Mat tmpImgRGB;
	cv::Mat filtered;

};

#endif //__RectangleFinder__
