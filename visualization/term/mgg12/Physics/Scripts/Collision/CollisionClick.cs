using UnityEngine;
using System.Collections;

public class CollisionClick : MonoBehaviour {
	string original;
	TextMesh mesh;

	void Start(){
		mesh = (TextMesh)GameObject.Find ("Instructions").GetComponent<TextMesh> ();
		original = mesh.text;
	}
	
	void OnMouseDown(){
		Application.LoadLevel ("PhysicsCollision");
	}

	void OnMouseOver(){
		mesh.text = "Learn about how collisions work";
	}

	void OnMouseExit(){
		mesh.text = original;
	}
	
	void Update(){
		
	}
}