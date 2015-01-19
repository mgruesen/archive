using UnityEngine;
using System.Collections;

public class SphereScript : MonoBehaviour {
	private Vector3 stationary;
	private Vector3 original;
	private Rigidbody body;
	// Use this for initialization
	void Start () {
		body = GetComponent<Rigidbody> ();
		body.useGravity = false;
		body.mass = 1.0f;
		original = body.position;
		stationary = new Vector3 (0, 0, 0);
	}

	public void Reset(){
		body.position = original;
		body.velocity = stationary;
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
