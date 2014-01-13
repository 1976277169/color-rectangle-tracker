#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class UnityCameraRenderer {

    public:
        void initRendering(int textureId, int width, int height);
        void renderCameraFrame(unsigned char* data);
        
    private:
        
        int textureId;
        int width;
        int height;
        
		unsigned char* watermark;
		
        void checkGlError(const char* operation);
		void renderWatermark(unsigned char* data);
};
