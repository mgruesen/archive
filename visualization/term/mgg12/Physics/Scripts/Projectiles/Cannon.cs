using UnityEngine;
using System.Collections;

public class Cannon : MonoBehaviour {
	private ParticleSystem fire;
	public GameObject round;
	private Munition munition;

	
	// Use this for initialization
	void Start () {
		munition = round.GetComponent<Munition> ();
		fire = GameObject.Find ("Explosion").GetComponent<ParticleSystem> ();
		GameObject.Find ("AngleValue").GetComponent<TextMesh> ().text = 
			string.Format ("{0:0}", this.transform.eulerAngles [2]);
	}

	public void ChangeAngleZ(float az){

		float x = this.transform.eulerAngles [0];
		float y = this.transform.eulerAngles [1];
		float z = this.transform.eulerAngles [2];
		if (z + az > 0) {
			this.transform.eulerAngles = new Vector3(x,y,z+az);
		}
	}

	public void ChangeAngle(Vector3 angle){
		this.transform.eulerAngles = angle;
		munition.ChangeAngle (angle);
	}

	public float GetAngleZ(){
		return this.transform.eulerAngles [2];
	}
	
	void OnMouseDown(){
		fire.Play ();
		munition.Fire ();
	}
	
	// Update is called once per frame
	void Update () {
	}
}
