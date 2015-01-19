using UnityEngine;
using System.Collections;

public class Munition : MonoBehaviour {
	public GameObject self;
	//Transform cannon;
	Vector3 originalPosition;
	Vector3 originalRotation;
	Vector3 force;

	void Start () {
		this.rigidbody.mass = 3.0f;
		originalPosition = this.transform.position;
		originalRotation = this.transform.eulerAngles;
		force = new Vector3 (40, 0, 0);
		self.SetActive (false);
	}

	public void ChangeForce(int v){
		float x = force [0];
		x = x + v > 0.0f ? x + v : 0.0f;
		float y = force [1];
		float z = force [2];		
		force = new Vector3 (x, y, z);
	}

	public float GetForceX(){
		return force [0];
	}

	public void ChangeAngle(Vector3 angle){
		this.transform.eulerAngles = angle;
	}

	public void ChangeMass(float m){
		float val = this.rigidbody.mass;
		if (val + m > 0.0)
			this.rigidbody.mass += m;
	}

	public float GetMass(){
		return this.rigidbody.mass;
	}

	public void Fire(){
		originalPosition = this.transform.position;
		originalRotation = this.transform.eulerAngles;
		this.rigidbody.velocity = transform.TransformDirection (force);
		self.SetActive (true);

	}

	public void Reset(){
		self.SetActive (false);
		new WaitForSeconds (1);
		this.transform.position = originalPosition;
		this.rigidbody.velocity = Vector3.zero;
		this.transform.eulerAngles = originalRotation;
	}

	void FixedUpdate(){
		if (!this.renderer.isVisible)
			Reset ();
	}
	
	void Update () {
	}

	void OnCollisionStay(Collision coll){
		if (coll.gameObject.name == "Target")
			this.GetComponent<ParticleSystem> ().Play ();
	}
}
