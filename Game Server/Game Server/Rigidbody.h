#pragma once
#include "include/glm/glm.hpp"
#include "Transform.h"

class Rigidbody {

public:
	Rigidbody();
	Rigidbody(Transform&);
	~Rigidbody();


	void addForce(float magnitude, glm::vec3 dir);
	void addForce(glm::vec3 force);

	void freeze();
	void freeze(bool t);
	void unfreeze();

	void addRotationalForce(glm::vec3 force);
	void addZRotation(float rotationZ);

	void update();

	glm::vec3 lAccel;
	glm::vec3 lVelocity;
	glm::vec3 rAccel;
	glm::vec3 rVelocity;
	
	Transform* parentTransform;

	float maxVelocity;
	float minVelocity;

	float lDrag;
	float rDrag;
	float aDrag;

	float mass;
	float gravAccel;

	bool gravity;

	bool keepUpdating;
	bool inAir;

};