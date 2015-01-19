using UnityEngine;
using System.Collections;

public class SpherePropertyChange : MonoBehaviour {

	public Color selected;
	public Color unselected;
	public GameObject myself;
	TextMesh self;
	FrictionSphere.MaterialType myType;
	FrictionSphere sphere;
	public SpherePropertyChange[] others;

	// Use this for initialization
	void Start () {
		self = myself.GetComponent<TextMesh> ();
		self.color = unselected;
		sphere = GameObject.Find ("Sphere").GetComponent<FrictionSphere> ();
		determineType ();
	}

	private void determineType(){
		switch (self.text) {
			case "SphereWood":
				myType = FrictionSphere.MaterialType.Wood;
				break;
			case "SphereMetal":
				myType = FrictionSphere.MaterialType.Metal;
				break;
			case "SphereIce":
				myType = FrictionSphere.MaterialType.Ice;
				break;
			case "SphereRubber":
				myType = FrictionSphere.MaterialType.Rubber;
				break;
			default:
				break;
		}
	}

	void OnMouseDown(){
		sphere.ChangeMaterial (myType);
		self.color = selected;
		foreach (SpherePropertyChange mesh in others) {
			mesh.Deselect();
		}
	}

	public void Deselect(){
		self.color = unselected;
	}
	// Update is called once per frame
	void Update () {
	
	}
}
