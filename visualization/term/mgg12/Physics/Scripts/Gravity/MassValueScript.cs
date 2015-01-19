using UnityEngine;
using System.Collections;

public class MassValueScript : MonoBehaviour {
	public Rigidbody body;

	private TextMesh mesh;
	private const float initial = 1.0f;
	private float value;

	// Use this for initialization
	void Start () {
		value = initial;
		mesh = GetComponent<TextMesh> ();
	}

	public void Adjust(float f){
		value = (float)(value + f > 0.0f ? value + f : 0.1);
		body.mass = value;
		mesh.text = value.ToString ();
	}

	public void Reset(){
		value = initial;
		body.mass = value;
		mesh.text = value.ToString ();
	}

	// Update is called once per frame
	void Update () {
		
	}
}
