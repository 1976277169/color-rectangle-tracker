package org.astri.unityrect;

import org.astri.arprocessing.camera.CameraHandler;

import android.os.Bundle;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;

import com.unity3d.player.UnityPlayerNativeActivity;

public class RectDetectNativeActivity extends UnityPlayerNativeActivity {

	private CameraHandler camera;
	private RectangleDetector rectangleDetector;
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		rectangleDetector = new RectangleDetector();
		
		SurfaceView preview = new SurfaceView(this);
		camera = new CameraHandler();
		camera.setPreviewHolder(preview);
		camera.setDataListener(rectangleDetector);
		
		addContentView(preview, new LayoutParams(1, 1));
	}

	protected void onPause() {
		super.onPause();
		camera.pauseCamera();
		mUnityPlayer.pause();

		if (isFinishing()) {
			mUnityPlayer.quit();
		}
	}

	protected void onResume() {
		super.onResume();
		mUnityPlayer.resume();
		camera.resumeCamera();
	}

	
}
