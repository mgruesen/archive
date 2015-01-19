using UnityEngine;
using System.Collections;

public class ToggleScript : MonoBehaviour {
	private FrictionSphere sphere;
	// Use this for initialization
	void Start () {
		sphere = GameObject.Find ("Sphere").GetComponent<FrictionSphere> ();
	}

	void OnMouseDown(){
		sphere.ApplyForce ();
	}

	// Update is called once per frame
	void Update () {
		
	}
}
