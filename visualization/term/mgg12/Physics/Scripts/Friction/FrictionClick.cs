using UnityEngine;
using System.Collections;

public class FrictionClick : MonoBehaviour {
	string original;
	TextMesh mesh;
	
	void Start(){
		mesh = (TextMesh)GameObject.Find ("Instructions").GetComponent<TextMesh> ();
		original = mesh.text;
	}

	void OnMouseDown(){
		Application.LoadLevel ("PhysicsFriction");
	}

	void OnMouseOver(){
		mesh.text = "Learn about how friction works";
	}
	
	void OnMouseExit(){
		mesh.text = original;
	}

	void Update(){

	}
}