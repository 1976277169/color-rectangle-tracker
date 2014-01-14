using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System.IO;

public class RectangleDetect : MonoBehaviour {

	#if UNITY_ANDROID
	[DllImport("rectangledetector")]
	private static extern void AR_InitBackgroundRender(int textureId, int width, int height);
	[DllImport("rectangledetector")]
	private static extern void AR_RenderBackgroundFrame();
	[DllImport("rectangledetector")]
	private static extern void AR_GetMarkerLocation([In,Out] int[] marker);
	[DllImport("rectangledetector")]
	private static extern void AR_InitColorRange(int minB, int maxB, int minG, int maxG, int minR, int maxR);
	#endif
	#if UNITY_IPHONE
	[DllImport("__Internal")]
	private static extern void AR_InitBackgroundRender(int textureId, int width, int height);
	[DllImport("__Internal")]
	private static extern void AR_RenderBackgroundFrame();
	[DllImport("__Internal")]
	private static extern void AR_GetMarkerLocation([In,Out] int[] marker);
	[DllImport("__Internal")]
	private static extern void AR_InitColorRange(int minB, int maxB, int minG, int maxG, int minR, int maxR);
	#endif

	private const int CAMERA_WIDTH = 640;
	private const int CAMERA_HEIGHT = 480;

	public GameObject backgroundPlane;
	Texture2D tex;

	private int[] markerCoordinates = new int[]{0,0,0,0,0};

	// Here are the min/max values for each RGB color component.
	// Value range is 0 - 255, min value must be smaller than max value.
	// Only objects which have color value within the specified range will be tracked.
	// You can also specify separate ranges for Android and iOS platforms
	public int minR;
	public int maxR;
	public int minG;
	public int maxG;
	public int minB;
	public int maxB;

	void Start(){
		CreateBackgroundTextureAndPassToPlugin();
	}

	private void CreateBackgroundTextureAndPassToPlugin() {
		
		// Create a texture for the background camera. 
		// Currently the only suported resolution is 640x480
		tex = new Texture2D(CAMERA_WIDTH, CAMERA_HEIGHT, TextureFormat.RGB565, false);
		
		tex.filterMode = FilterMode.Trilinear;
		// Call Apply() so it's actually uploaded to the GPU
		tex.Apply();
		
		backgroundPlane.renderer.material.mainTexture = tex;
		
		// Pass texture pointer to the plugin and initialize the
		// background rendering
		AR_InitBackgroundRender(tex.GetNativeTextureID(), CAMERA_WIDTH, CAMERA_HEIGHT);
		AR_InitColorRange(minB, maxB, minG, maxG, minR, maxR);
	}
	
	// Update is called once per frame
	void Update () {
		AR_GetMarkerLocation(markerCoordinates);
	}

	public bool transformToLocation(GameObject model){

		if(markerCoordinates[0] > 0){
			float y = markerCoordinates[1] * (100f / CAMERA_WIDTH) - 50f;
			float x = markerCoordinates[2] * (80f / CAMERA_HEIGHT) - 40f;

			//int width = markerCoordinates[3];
			//int height = markerCoordinates[4];
			//float scale = (float)height / (float)CAMERA_HEIGHT;
			//Debug.Log("x: " + x + ", y: " + y);
			model.transform.position = new Vector3(-x, -y, model.transform.position.z);
			//model.transform.localScale = new Vector3(scale, scale, scale);
			return true;
		}
		return false;
	}

	void OnPostRender() {
		AR_RenderBackgroundFrame();
	}

}
