
#include "RectangleDetector.hpp"


std::auto_ptr<RectangleDetectionFacade> createRectangleDetection()
{
    return std::auto_ptr<RectangleDetectionFacade>(new RectangleDetector());
}


void RectangleDetector::initProcessing(int frameWidth, int frameHeight){
    tmpImgRGB = cv::Mat::zeros(480, 640, CV_8UC3);
    BGRAMat = cv::Mat::zeros(480, 640, CV_8UC4);
}

void RectangleDetector::initColorRange(int miB, int maB, int miG, int maG, int miR, int maR){
    rectFinder.setColorRange(miB, maB, miG, maG, miR, maR);
}

void RectangleDetector::processFrame(const BGRAVideoFrame& frame){
    
    BGRAMat.data = frame.data;
    rectFinder.detectRectangles(BGRAMat, rectArray);

}

void RectangleDetector::getLocationArray(int* array){

    if(rectArray[0] > 0){
        array[0] = 1;
        array[1] = rectArray[1];
        array[2] = rectArray[2];
        array[3] = rectArray[3];
        array[4] = rectArray[4];
    } else {
        array[0] = -1;
    }

}

