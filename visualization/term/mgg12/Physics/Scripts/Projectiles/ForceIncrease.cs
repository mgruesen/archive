using UnityEngine;
using System.Collections;

public class ForceIncrease : MonoBehaviour {
	public GameObject munition;
	private Munition munitionScript;
	private TextMesh forceValue;
	// Use this for initialization
	void Start () {
		forceValue = GameObject.Find ("ForceValue").GetComponent<TextMesh> ();
		munitionScript = munition.GetComponent<Munition> ();
	}

	void OnMouseDown(){
		munitionScript.ChangeForce (1);
		forceValue.text = string.Format ("{0:0}", munitionScript.GetForceX ());
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
