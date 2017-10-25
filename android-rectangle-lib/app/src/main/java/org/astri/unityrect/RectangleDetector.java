package org.astri.unityrect;

import org.astri.camerahandler.CameraDataListener;

public class RectangleDetector implements CameraDataListener {

	static {
        // Load JNI library
        System.loadLibrary("opencv_java");
        System.loadLibrary("rectangledetector");
    };


	@Override
	public void receiveCameraFrame(byte[] bytes, int width, int height, boolean isBackCamera, int imageFormat, int imageOrientation) {
		setCameraFrame(bytes);
	}

	@Override
	public void receivePhotoFrame(byte[] bytes, int i, int i1) {

	}

	@Override
	public void receiveMarkerFrame(byte[] bytes, int i, int i1, float v) {

	}

	private static native void setCameraFrame(byte[] frame);

}
