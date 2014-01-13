using UnityEngine;
using System.Collections;


public class TrackableScript : MonoBehaviour {

	// set this to reference ARCamera
	public RectangleDetect rectScript;
	
	// Use this for initialization
	void Start () {

	}
	
	// Update is called once per frame
	void Update () {
		bool isTracking = rectScript.transformToLocation(this.gameObject);
	}
}
