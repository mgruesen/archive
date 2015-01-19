using UnityEngine;
using System.Collections;

public class FrictionSphere : MonoBehaviour {

	Transform self;
	Vector3 initPos;
	Vector3 initRot;
	Vector3 currentForce;
	TextMesh forceVal;
	SphereCollider myCollider;
	public Color select;
	public Color unselect;

	public enum MaterialType { Wood, Metal, Ice, Rubber };

	// Use this for initialization
	void Start () {
		self = GameObject.Find ("Sphere").GetComponent<Transform> ();
		myCollider = GameObject.Find ("Sphere").GetComponent<SphereCollider> ();
		forceVal = GameObject.Find ("ForceValue").GetComponent<TextMesh> ();
		initPos = self.position;
		initRot = self.eulerAngles;
		currentForce = Vector3.zero;
		changeMaterial (new PhysicMaterial ("Wood"));
	}

	public void ChangeMaterial(MaterialType type){
		switch (type) {
			case MaterialType.Ice:
				changeMaterial(new PhysicMaterial("Ice"));
				break;
			case MaterialType.Metal:
				changeMaterial(new PhysicMaterial("Metal"));
				break;
			case MaterialType.Rubber:
				changeMaterial(new PhysicMaterial("Rubber"));
				break;
			case MaterialType.Wood:
				changeMaterial(new PhysicMaterial("Wood"));
				break;
			default: break;
		}
	}

	private void changeMaterial(PhysicMaterial mat){
		rigidbody.isKinematic = true;
		rigidbody.Sleep ();
		myCollider.material = myCollider.sharedMaterial = mat;
		rigidbody.isKinematic = false;
		rigidbody.WakeUp ();
	}

	public void ChangeForce(float f){
		currentForce [0] =(float) (currentForce [0] + f > 0.0 ? currentForce [0] + f : 0.0);
		forceVal.text = string.Format("{0:0}", currentForce[0]);
	}

	public void ApplyForce(){
		self.rigidbody.AddForce (currentForce);
	}

	public void Reset(){
		self.rigidbody.useGravity = false;
		self.position = initPos;
		self.eulerAngles = initRot;
		self.rigidbody.velocity = Vector3.zero;
		self.rigidbody.angularVelocity = Vector3.zero;
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
