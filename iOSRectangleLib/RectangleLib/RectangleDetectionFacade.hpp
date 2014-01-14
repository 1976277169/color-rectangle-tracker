
#ifndef RectangleDetectionFacade_h
#define RectangleDetectionFacade_h

#include <vector>
#include <memory>
#include <stdio.h>
#include <iostream>

#include "BGRAVideoFrame.h"

class RectangleDetectionFacade
{
public:
    virtual ~RectangleDetectionFacade() {}
    
    virtual void initProcessing(int frameWidth, int frameHeight) = 0;
    virtual void initColorRange(int minB, int maxB, int minG, int maxG, int minR, int maxR) = 0;
    virtual void processFrame(const BGRAVideoFrame& frame) = 0;
    virtual void getLocationArray(int* array) = 0;

};

std::auto_ptr<RectangleDetectionFacade> createRectangleDetection();

#endif
