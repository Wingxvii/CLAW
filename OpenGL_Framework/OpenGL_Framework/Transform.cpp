#include "Transform.h"

Transform::Transform()
	: m_pScale(1.0f),
	color(glm::vec4(1.0f))
{
}

Transform::~Transform() {}

void Transform::setPosition(glm::vec3 newPosition)
{
	m_pLocalPosition = newPosition;
}

void Transform::setRotationAngleX(float newAngle)
{
	m_pRotX = newAngle;
}

void Transform::setRotationAngleY(float newAngle)
{
	m_pRotY = newAngle;
}

void Transform::setRotationAngleZ(float newAngle)
{
	m_pRotZ = newAngle;
}

void Transform::setScale(float newScale)
{
	m_pScale = newScale;
}

glm::vec3 Transform::getPosition()
{
	return m_pLocalPosition;
}

float Transform::getRotationAngleY()
{
	return m_pRotY;
}

glm::mat4 Transform::getLocalToWorldMatrix()
{
	return m_pLocalToWorldMatrix;
}

void Transform::update(float dt)
{
	// Create 4x4 transformation matrix

	// Create rotation matrix

	glm::mat4 rx = glm::mat4(1.0);
	glm::mat4 ry = glm::mat4(1.0);
	glm::mat4 rz = glm::mat4(1.0);

	//rx.RotateX(m_pRotX);
	rx = glm::rotate(rx, glm::radians(m_pRotX), glm::vec3{ 1.0f,0.0f,0.0f });
	//ry.RotateY(m_pRotY);
	ry = glm::rotate(ry, glm::radians(m_pRotY), glm::vec3{ 0.0f,1.0f,0.0f });
	//rz.RotateZ(m_pRotZ);
	rz = glm::rotate(rz, glm::radians(m_pRotZ), glm::vec3{ 0.0f,0.0f,1.0f });
	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	m_pLocalRotation = rz * ry * rx;

	// Create translation matrix
	glm::mat4 tran = glm::mat4(1.0);
	//tran.Translate(m_pLocalPosition);
	tran = glm::translate(tran, m_pLocalPosition);

	// Create scale matrix
	glm::mat4 scale = glm::mat4(1.0);
	//scale.Scale(m_pScale);
	scale = scale * m_pScale;

	// Combine all above transforms into a single matrix
	// This is the local transformation matrix, ie. where is this game object relative to it's parent
	// If a game object has no parent (it is a root node) then its local transform is also it's global transform
	// If a game object has a parent, then we must apply the parent's transform
	m_pLocalToWorldMatrix = tran * m_pLocalRotation * scale;
}