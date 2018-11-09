#include "Rigidbody.h"
#include <string>

Rigidbody::Rigidbody()
{
	lAccel = glm::vec3(0,0,0);
	lVelocity = glm::vec3(0, 0, 0);
	rAccel = glm::vec3(0, 0, 0);
	rVelocity = glm::vec3(0, 0, 0);

	maxVelocity = std::numeric_limits<float>::max();
	minVelocity = 0.0f;

	lDrag = 0.0f;
	rDrag = 0.9f;
	aDrag = 0.0f;
	mass = 1.0f;
	gravAccel = 1.0f;

	gravity = false;
	inAir = false;
	keepUpdating = true;

}

Rigidbody::Rigidbody(Transform& parent)
{
	lAccel = glm::vec3(0, 0, 0);
	lVelocity = glm::vec3(0, 0, 0);
	rAccel = glm::vec3(0, 0, 0);
	rVelocity = glm::vec3(0, 0, 0);

	maxVelocity = std::numeric_limits<float>::max();
	minVelocity = 0.0f;

	lDrag = 0.05f;
	rDrag = 0.9f;
	aDrag = 0.0f;
	mass = 1.0f;
	gravAccel = 1.0f;

	gravity = false;
	inAir = false;

	keepUpdating = true;
	//prob an error ************************************************************************
	parentTransform = &parent;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::addForce(float magnitude, glm::vec3 dir)
{
	lAccel += (dir * magnitude) / mass;
}

void Rigidbody::addForce(glm::vec3 force)
{
	lAccel += force / mass;
}

void Rigidbody::freeze()
{
	keepUpdating = false;
}

void Rigidbody::freeze(bool t)
{
	keepUpdating = t;
}

void Rigidbody::unfreeze()
{
	keepUpdating = true;
}

void Rigidbody::addRotationalForce(glm::vec3 force)
{
	rAccel += force;
}

void Rigidbody::addZRotation(float rotationZ)
{
	rAccel.z += rotationZ;
}

void Rigidbody::update()
{
	//checks if update is running
	if (keepUpdating) {

		//updates velocity
		if (inAir) {
			lVelocity = lVelocity * (1 - aDrag) + lAccel;
		}
		else {
			lVelocity = lVelocity * (1 - lDrag) + lAccel;
		}

		rVelocity = rVelocity *(1-rDrag) + rAccel;

		//restricts velocity range
		if (glm::length(lVelocity) > maxVelocity) {
			lVelocity = glm::normalize(lVelocity) * maxVelocity;
		}
		if (glm::length(lVelocity) < minVelocity) {
			lVelocity = glm::normalize(lVelocity) * minVelocity;
		}

		//applies gravity
		if (gravity) {
			lVelocity += glm::vec3(0,-gravAccel/(mass + 0.0001), 0);
		}

		//moves position
		parentTransform->position += lVelocity;
		parentTransform->rotation += rVelocity;

		lAccel = glm::vec3(0.0f, 0.0f, 0.0f);
		rAccel = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}
