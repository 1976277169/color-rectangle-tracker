#ifndef __RectangleFinder__
#define __RectangleFinder__

#include "Logwrapper.h"
#include "opencv2/opencv.hpp"
#include <vector>

class RectangleFinder {
public:
	RectangleFinder();
    void setColorRange(int minB, int maxB, int minG, int maxG, int minR, int maxR);
    // input image is a BGRA or BGR565 matrix
	void detectRectangles(cv::Mat& image, int* rectArray);

private:

	void init();
	void findSquares(const cv::Mat& image, std::vector<std::vector<cv::Point> >& squares, std::vector<cv::Rect>& rectangles, int rectArray[]);
	void drawSquares(cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares);
	void drawRectangles(cv::Mat& image, const std::vector<cv::Rect>& rectangles);

	void boundingBoxes(std::vector<std::vector<cv::Point> > contours, std::vector<cv::Rect>& rectangles);
	void filterRectangles(std::vector<std::vector<cv::Point> > contours, std::vector<std::vector<cv::Point> >& squares);
	double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	void gendiamond(cv::Mat& dst, std::vector<int>& list);

	cv::Mat diamond;
	cv::Mat tmpImgRGB;
	cv::Mat filtered;
    
    int minB;
    int maxB;
    int minG;
    int maxG;
    int minR;
    int maxR;

};

#endif //__RectangleFinder__
