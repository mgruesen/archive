using UnityEngine;
using System.Collections;

public class Target : MonoBehaviour {
	GameObject self;
	private Vector3 originalPosition;
	private Vector3 originalRotation;
	// Use this for initialization
	void Start () {
		self = GameObject.Find ("Target");
		originalPosition = self.transform.position;
		originalRotation = self.transform.eulerAngles;
	}

	public void Reset(){
		new WaitForSeconds (1);
		self.rigidbody.velocity = Vector3.zero;
		self.rigidbody.angularVelocity = Vector3.zero;
		self.transform.position = originalPosition;
		self.transform.eulerAngles = originalRotation;

	}

	void FixedUpdate(){
		if (!self.renderer.isVisible)
			Reset ();
	}

	// Update is called once per frame
	void Update () {
	
	}
}
