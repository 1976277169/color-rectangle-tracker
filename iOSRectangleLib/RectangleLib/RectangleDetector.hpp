
#ifndef RectangleDetector_h
#define RectangleDetector_h


#include <vector>
#include <opencv2/opencv.hpp>


#include "BGRAVideoFrame.h"
#include "RectangleDetectionFacade.hpp"
#include "RectangleFinder.h"


#define NO_OF_MESH 500

class RectangleDetector : public RectangleDetectionFacade
{
public:
    
    void initProcessing(int frameWidth, int frameHeight);
    void initColorRange(int minB, int maxB, int minG, int maxG, int minR, int maxR);
    void processFrame(const BGRAVideoFrame& frame);
    void getLocationArray(int* array);
    
private:
    
    RectangleFinder rectFinder;
    cv::Mat BGRAMat;
    cv::Mat tmpImgRGB;
    int rectArray[5];
    
};

#endif
