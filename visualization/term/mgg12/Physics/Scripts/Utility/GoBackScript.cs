using UnityEngine;
using System.Collections;

public class GoBackScript : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}

	void OnMouseDown(){
		Application.LoadLevel ("PhysicsMain");
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
