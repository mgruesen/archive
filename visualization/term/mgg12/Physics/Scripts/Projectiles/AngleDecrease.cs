using UnityEngine;
using System.Collections;

public class AngleDecrease : MonoBehaviour {
	private Cannon cannon;
	private TextMesh angleValue;

	void Start () {
		cannon = GameObject.Find ("Cannon").GetComponent<Cannon> ();
		angleValue = GameObject.Find ("AngleValue").GetComponent<TextMesh> ();
	}
	
	void OnMouseDown(){
		cannon.ChangeAngleZ (-1);
		angleValue.text = string.Format("{0:0}", cannon.GetAngleZ ());
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
