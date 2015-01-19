using UnityEngine;
using System.Collections;

public class CollisionScript : MonoBehaviour {
	public Rigidbody body;

	void Start(){
		body.collisionDetectionMode = CollisionDetectionMode.Continuous;
		body.detectCollisions = true;
	}

	void OnCollisionEnter(){

	}
	
	void Update(){
		
	}
}