package org.astri.unityrect;

import org.astri.arprocessing.camera.CameraDataListener;

public class RectangleDetector implements CameraDataListener {

	static {
        // Load JNI library
        System.loadLibrary("opencv_java");
        System.loadLibrary("rectangledetector");
    };

	
	@Override
	public void receiveCameraFrame(byte[] data, int width, int height) {
		//Log.d("RD", "received frame");
		setCameraFrame(data);
	}

	private static native void setCameraFrame(byte[] frame);
	
}
