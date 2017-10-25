package org.astri.rectangledetector;

import org.astri.camerahandler.CameraDataListener;
import org.astri.camerahandler.CameraHandler;
import org.astri.rendering.RenderSurface;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;

public class MainActivity extends Activity implements CameraDataListener{

	static {
		System.loadLibrary("rectangledetector");
	}
	
	private static final String TAG = "TemplateActivity";
	
	private CameraHandler camera;
	
	@Override
	public void onCreate(Bundle bundle) {

		super.onCreate(bundle);

		// This if using rendering on Android side
		setContentView(R.layout.main);

		initCamera();
	}
	
	private void initCamera(){
		
		// This if using rendering on Android side
		SurfaceView cameraPreview = (SurfaceView) findViewById(R.id.cameraPreview);
		
		// This if using rendering on native side
		RenderSurface sv = new RenderSurface(this);
		addContentView(sv, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		sv.bringToFront();
		
		camera = new CameraHandler(getApplicationContext());
		camera.setPreviewHolder(cameraPreview);
		camera.setDataListener(this);
		Log.d(TAG, "finished camera init");
	}
	
	@Override
	public void onResume() {
		super.onResume();
		camera.resumeCamera();
		startProcessing();

	}

	@Override
	public void onPause() {
		
		stopProcessing();
		camera.pauseCamera();
		super.onPause();
	}

	
	@Override
	public void receiveCameraFrame(byte[] data, int width, int height, boolean backCamera, int imageFormat, int imageOrientation) {
		setCameraFrame(data);
	}

	@Override
	public void receivePhotoFrame(byte[] bytes, int i, int i1) {
	}

	@Override
	public void receiveMarkerFrame(byte[] bytes, int i, int i1, float v) {
	}

	private native void startProcessing();
	private native void stopProcessing();
	private native void setCameraFrame(byte[] frame);

}
