#include "Camera.h"

#include <random>

Camera::Camera()
{
	int number = rand() % 2;
	Camera((ProjectionType)number);
	m_entityType = (int)EntityTypes::CAMERA;
}

Camera::Camera(ProjectionType projType)
{
	if (projType == ProjectionType::Perspective)
		perspective(60.0f, 1.0f, 0.1f, 100.0f);
	else
		orthographic(-10, 10, -10, 10, -100, 100);
	m_entityType = (int)EntityTypes::CAMERA;
}

void Camera::perspective(float fovy, float aspect, float zNear, float zFar)
{
	projectionType = ProjectionType::Perspective;
	projection = glm::perspective(
		fovy, aspect,
		zNear, zFar);
}

void Camera::orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	projectionType = ProjectionType::Orthographic;
	projection = glm::ortho(
		left, right,
		bottom, top,
		zNear, zFar);
}

glm::mat4 Camera::getView() const
{
	return transform->getLocalToWorldMatrix();
}

glm::mat4 Camera::getProjection() const
{
	return projection;
}