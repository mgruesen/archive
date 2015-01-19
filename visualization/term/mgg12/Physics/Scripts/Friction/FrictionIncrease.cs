using UnityEngine;
using System.Collections;

public class FrictionDecrease : MonoBehaviour {

	FrictionSphere sphere;

	// Use this for initialization
	void Start () {
		sphere = GameObject.Find ("Sphere").GetComponent<FrictionSphere> ();
	}

	void OnMouseDown(){
		sphere.ChangeForce (-1.0f);
	}

	// Update is called once per frame
	void Update () {
	
	}
}
