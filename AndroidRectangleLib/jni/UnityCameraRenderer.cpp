#include "UnityCameraRenderer.h"
#include "../../RectangleFinder/Logwrapper.h"
//#include "astri_img.h"

void UnityCameraRenderer::initRendering(int texId, int w, int h){

    textureId = texId;
    width = w;
    height = h;
}
/*
void UnityCameraRenderer::renderWatermark(uint8_t* data){

	int topPadding = 50;
	int leftPadding = 50;
	int stride = 1280;

	int imgHeight = 168;
	int imgStride = 640;

	int startRow = topPadding * stride;
	int imageData = 0;

	for(int i = 0; i < imgHeight; i++){
		int row = startRow + (i * stride);
		for(int j = 0; j < imgStride; j++){
			uint8_t pixel = astriImageData[imageData];
			imageData++;

			if(pixel != 0xFF){
				data[row + leftPadding + j] = pixel;
			}
		}
	}

}
*/
void UnityCameraRenderer::renderCameraFrame(unsigned char* data){

    if(!data){
    	LOGW("Tried to render empty frame");
        return;
    }

	//renderWatermark(data);

    checkGlError("start render");

    //print_hexi(data, 8);

    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
    //        GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                    GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data );

    checkGlError("glteximage");

}

void UnityCameraRenderer::checkGlError(const char* operation){
    for (GLint error = glGetError(); error; error = glGetError()){
        LOGE("after %s() glError (0x%x)", operation, error);
    }
}
