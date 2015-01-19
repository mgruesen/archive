using UnityEngine;
using System.Collections;

public class MassIncrease : MonoBehaviour {
	public GameObject munition;
	private TextMesh massValue;
	private Munition munitionScript;
	void Start () {
		munitionScript = munition.GetComponent<Munition> ();
		massValue = GameObject.Find ("MassValue").GetComponent<TextMesh> ();
	}

	void OnMouseDown(){
		munitionScript.ChangeMass (0.5f);
		massValue.text = string.Format("{0:0.0#}", munitionScript.GetMass());
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
