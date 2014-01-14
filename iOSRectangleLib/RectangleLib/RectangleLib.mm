//
//  RectangleLib.m
//  RectangleLib
//
//  Created by VIEW Laboratory on 13/1/14.
//  Copyright (c) 2014 ASTRI. All rights reserved.
//

#import "RectangleLib.h"
#import "BackgroundRenderer.h"
//#import "RectangleDetectionFacade.hpp"
#import "RectangleDetector.hpp"

@implementation RectangleLib


RectangleLib* pt;

int textureId;
int cameraWidth = 640;
int cameraHeight = 480;
int faceArray[5];

VideoSource * m_videoSource;
BGRAVideoFrame renderBuf;
NSObject* renderLock;
bool frameReady = false;

std::auto_ptr<RectangleDetectionFacade> m_pipeline;

-(id)init {
    self = [super init];
    printf("init FaceLib");
    m_videoSource = [[VideoSource alloc]init];
    m_videoSource.delegate = self;
    
    renderLock = [[NSObject alloc] init];
    renderBuf.data = (uint8_t*) calloc(cameraWidth * cameraHeight * 4, sizeof(uint8_t));
    faceArray[0] = 0;
    [m_videoSource startWithDevicePosition:AVCaptureDevicePositionBack];
    
    return self;
}


+(void) initCamera {
    printf("init camera \n");
    if(pt == NULL){
        pt = [[RectangleLib alloc] init];
    }
    
}

-(void)frameReady:(BGRAVideoFrame) frame{
    
    @synchronized(renderLock){
        renderBuf.width = frame.width;
        renderBuf.height = frame.height;
        renderBuf.stride = frame.stride;
        memcpy(renderBuf.data, frame.data, cameraWidth * cameraHeight * 4);
        frameReady = true;
        
        m_pipeline->processFrame(renderBuf);
    }
}

void AR_InitBackgroundRender(int textId, int w, int h) {
    
    m_pipeline = createRectangleDetection();
    m_pipeline->initProcessing(w, h);
    
    printf("received texture id: %d \n", textId);
    textureId = textId;
    cameraWidth = w;
    cameraHeight = h;
    
    [RectangleLib initCamera];
    
}

void AR_InitColorRange(int minB, int maxB, int minG, int maxG, int minR, int maxR){
    
    m_pipeline->initColorRange(minB, maxB, minG, maxG, minR, maxR);
    
}

void AR_RenderBackgroundFrame(){
    @synchronized(renderLock){
        [BackgroundRenderer renderBackground:textureId : renderBuf.data];
    }
}

void AR_GetMarkerLocation(int* array){
    m_pipeline->getLocationArray(array);
}


@end