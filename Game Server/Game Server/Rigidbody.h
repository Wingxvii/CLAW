#pragma once
#include "include/glm/glm.hpp"


class Rigidbody {

public:
	Rigidbody();
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
	glm::vec3 position;
	glm::vec3 rAccel;
	glm::vec3 rVelocity;
	glm::vec3 rotation;

	float maxVelocity;
	float minVelocity;

	float lDrag;
	float rDrag;
	float mass;
	float gravAccel;

	bool gravity;

	bool keepUpdating;


};