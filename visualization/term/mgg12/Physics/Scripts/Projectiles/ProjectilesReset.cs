using UnityEngine;
using System.Collections;


public class ProjectilesReset : MonoBehaviour {
	public GameObject munition;
	public GameObject target;
	// Use this for initialization
	void Start () {
	
	}

	void OnMouseDown(){
		try{
			munition.GetComponent<Munition> ().Reset ();
		} 
		catch(System.Exception){}

		try{
			target.GetComponent<Target> ().Reset ();
		}
		catch(System.Exception){}
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
