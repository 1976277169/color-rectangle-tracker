package org.astri.rendering;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class RenderSurface extends GLSurfaceView {
	
	public RenderSurface(Context context) {
		super(context);
		init();
	}

	public RenderSurface(Context context, AttributeSet attribs) {
	    super(context, attribs);
	    init();
	}
	
	private void init() {
		setZOrderMediaOverlay(true);
		setEGLConfigChooser(8, 8, 8, 8, 16, 0);
		getHolder().setFormat(PixelFormat.TRANSLUCENT);
		setEGLContextClientVersion( 2 );
		setRenderer(new ARRenderer(getContext()));
		//setVisibility(View.GONE);
	}

}

