#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>
#include <string>

class Transform
{
public:
	float m_pScale;

	float m_pRotX = 0.0f;
	float m_pRotY = 0.0f;
	float m_pRotZ = 0.0f; // local rotation angles

	glm::vec3 m_pLocalPosition;

	// TODO: Compute these matrices in the Update function
	glm::mat4 m_pLocalRotation;
	glm::mat4 m_pLocalToWorldMatrix;

	Transform();
	~Transform();

	void setPosition(glm::vec3 newPosition);
	void setRotationAngleX(float newAngle);
	void setRotationAngleY(float newAngle);
	void setRotationAngleZ(float newAngle);
	void setScale(float newScale);

	glm::vec3 getPosition();
	float getRotationAngleY();

	glm::mat4 getLocalToWorldMatrix();

	// TODO: Implement this update function
	virtual void update(float dt);	
	
	// Other Properties
	std::string name;
	glm::vec4 color;
};