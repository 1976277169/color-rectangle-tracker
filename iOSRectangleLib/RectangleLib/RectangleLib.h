//
//  RectangleLib.h
//  RectangleLib
//
//  Created by VIEW Laboratory on 13/1/14.
//  Copyright (c) 2014 ASTRI. All rights reserved.
//
#ifndef RectangleLib_h
#define RectangleLib_h

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "VideoSource.h"

@interface RectangleLib : UIViewController<VideoSourceDelegate>
{
    
}

+(void) initCamera;

extern "C" {
    
    void AR_InitBackgroundRender(int textureId, int width, int height);
    void AR_InitColorRange(int minB, int maxB, int minG, int maxG, int minR, int maxR);
    void AR_RenderBackgroundFrame();
    void  AR_GetMarkerLocation(int* array);

}

@end

#endif

