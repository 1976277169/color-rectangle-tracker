#ifndef CAMERA_RENDERER_H
#define CAMERA_RENDERER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class CameraRenderer {

public:

	CameraRenderer(bool npotTextures = false);
	virtual ~CameraRenderer();

	bool Init();
	bool InitGPU();
	void UnInit();
	bool Render(const uint8_t* img, uint16_t w, uint16_t h);


private:

	bool CreateShaderProgram();
	bool CreateTextures();
	bool UpdateTextures(const uint8_t* rgbImg, uint16_t w, uint16_t h);

	bool mInitialized;
	bool mNPOTTextures;

	GLuint mWidth;
	GLuint mHeight;

	GLuint mTextureId[1];

	static GLfloat mVertices[];
	static const GLfloat mLandscapeVertices[];

	GLuint mProgramObject;
	GLint mGL_u_ImgRGBLoc;
	GLint mGL_av4_PositionLoc;
	GLint mGL_av2_TexCoordLoc;

};

#endif
