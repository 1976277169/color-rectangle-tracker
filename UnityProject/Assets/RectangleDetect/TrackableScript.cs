using UnityEngine;
using System.Collections;


public class TrackableScript : MonoBehaviour {

	// set this to reference ARCamera
	public RectangleDetect rectScript;
	
	// Use this for initialization
	void Start () {
		// When using scaling, if object has a collider, better
		// remove it here, otherwise performance will bevery slow.
		//Destroy(this.gameObject.renderer.collider);
	}
	
	// Update is called once per frame
	void Update () {
		bool isTracking = rectScript.transformToLocation(this.gameObject);
	}
}
