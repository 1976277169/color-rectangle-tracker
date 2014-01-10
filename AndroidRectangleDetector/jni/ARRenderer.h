#ifndef AR_RENDERER_H
#define AR_RENDERER_H

#include <jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

extern "C" {

   JNIEXPORT void JNICALL Java_org_astri_rendering_ARRenderer_initRendering
   (
      JNIEnv*  env,
      jobject  obj
   );

   JNIEXPORT void JNICALL Java_org_astri_rendering_ARRenderer_surfaceChanged
   (
      JNIEnv* env, 
      jobject obj,  
      jint x,
      jint y,
      jint width, 
      jint height
   );

   JNIEXPORT void JNICALL Java_org_astri_rendering_ARRenderer_render
   (
      JNIEnv*  env,
      jobject  obj
   );
    
   JNIEXPORT void JNICALL Java_org_astri_rendering_ARRenderer_renderObject
   (
		   JNIEnv*  env,
		   jobject  obj
   );

};

#endif
