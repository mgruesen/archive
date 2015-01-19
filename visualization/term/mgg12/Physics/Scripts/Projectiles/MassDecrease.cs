using UnityEngine;
using System.Collections;

public class MassDecrease : MonoBehaviour {
	public GameObject munition;
	private Munition munitionScript;
	private TextMesh massValue;
	// Use this for initialization
	void Start () {
		munitionScript = munition.GetComponent<Munition> ();
		massValue = GameObject.Find ("MassValue").GetComponent<TextMesh> ();
	}
	
	void OnMouseDown(){
		munitionScript.ChangeMass (-0.5f);
		massValue.text = string.Format("{0:0.0#}", munitionScript.GetMass());
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
