using UnityEngine;
using System.Collections;

public class ProjectilesClick : MonoBehaviour {
	string original;
	TextMesh mesh;
	
	void Start(){
		mesh = (TextMesh)GameObject.Find ("Instructions").GetComponent<TextMesh> ();
		original = mesh.text;
	}
	
	void OnMouseDown(){
		Application.LoadLevel ("PhysicsProjectiles");
	}

	void OnMouseOver(){
		mesh.text = "Learn about how projectiles work";
	}
	
	void OnMouseExit(){
		mesh.text = original;
	}

	void Update(){
		
	}
}