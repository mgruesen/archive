using UnityEngine;
using System.Collections;

public class IncreaseMassScript : MonoBehaviour
{
	public TextMesh mesh;
	
	// Use this for initialization
	void Start ()
	{
	}

	void OnMouseDown(){
		mesh.GetComponent<MassValueScript> ().Adjust (0.1f);
	}

	// Update is called once per frame
	void Update ()
	{
	}
}

