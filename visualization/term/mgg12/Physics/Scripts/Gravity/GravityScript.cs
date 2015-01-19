using UnityEngine;
using System.Collections;

public class GravityScript : MonoBehaviour {
	public Rigidbody sphere;
	// Use this for initialization
	void Start () {
		Debug.Log ("start");
	}

	void OnMouseDown () {
		TextMesh mesh = GetComponent<TextMesh> ();
		switch (mesh.text) {
			case "Gravity On!":
					mesh.text = "Gravity";
					sphere.useGravity = false;
					new WaitForSeconds (1);					
					mesh.text += " Off";
					break;
			case "Gravity Off":
					mesh.text = "Gravity";
					sphere.useGravity = true;					
					new WaitForSeconds (1);
					mesh.text += " On!";
					break;
			default:
					break;
		}
	}

	// Update is called once per frame
	void Update () {
	
	}
}
