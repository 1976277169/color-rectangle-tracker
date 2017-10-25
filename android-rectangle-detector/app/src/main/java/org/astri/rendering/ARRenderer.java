package org.astri.rendering;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;

public class ARRenderer implements GLSurfaceView.Renderer { //, EGLContextFactory {

	
	public ARRenderer(Context context){
		
	}
	
	@Override
	public void onDrawFrame(GL10 gl) {
		//Log.d("", "on draw frame");
		//GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
		render();
		renderObject();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int w, int h) {
		Log.d("", "surface changed w: " + w + ", h: " + h);
		
		//int surface_width = h * 4 / 3;
		//surfaceChanged((w - surface_width) / 2, 0, surface_width, h);
		//gl.glViewport((w - surface_width) / 2, 0, surface_width, h);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		Log.d("", "surface created");
		initRendering();
	}
	
	public native void initRendering();
	public native void surfaceChanged(int x, int y, int w, int h);
	public native void render();
	public native void renderObject();
	
}
