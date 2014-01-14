//
//  BackgroundRenderer.m
//  Unity-iPhone
//
//  Created by VIEW Laboratory on 25/6/13.
//
//

#import "BackgroundRenderer.h"
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@implementation BackgroundRenderer

void printHex(uint8_t* data){
    printf("3rd Data: ");
    for(int i = 0; i < 32; i++){
        printf("%02x", data[i]);
    }
    printf("\n");
}

+(void) renderBackground: (int) textureId : (uint8_t*)frame {
    
    if(frame == NULL){
        return;
    }
    
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 480, 0, GL_BGRA, GL_UNSIGNED_BYTE, frame);
    
    int glErCode = glGetError();
    if (glErCode != GL_NO_ERROR)
    {
        printf("GL Error: %d", glErCode);
    }
    
}

@end
