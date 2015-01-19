using UnityEngine;
using System.Collections;

public class ResetScript : MonoBehaviour {
	public Rigidbody body;
	// Use this for initialization
	void Start () {

	}

	void OnMouseDown(){
		body.GetComponent<SphereScript> ().Reset ();
	}

	// Update is called once per frame
	void Update () {
	
	}
}
