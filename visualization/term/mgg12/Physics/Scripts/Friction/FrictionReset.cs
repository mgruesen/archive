using UnityEngine;
using System.Collections;

public class FrictionReset : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}

	void OnMouseDown(){
		GameObject.Find ("Sphere").GetComponent<FrictionSphere> ().Reset ();
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
