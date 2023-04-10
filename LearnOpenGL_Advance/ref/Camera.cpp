#include "Camera.h"

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position,position + front,up);
}

void Camera::processKeyBoard(Camera_Movement direction, float deltatime)
{
	float velocity = movementSpeed * deltatime;
	if (direction == FORWARD)
		position += velocity * front;
	if (direction == BACKWARD)
		position -= velocity * front;
	if (direction == LEFT)
		position -= velocity * right;
	if (direction == RIGHT)
		position += velocity * right;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch)
	{
		if (pitch > 85.0f)
			pitch = 85.0f;
		if (pitch < -85.0f)
			pitch = -85.0f;
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	fov -= yoffset;
	if (fov < 45.0f)
		fov = 45.0f;
	if (fov > 120.0f)
		fov = 120.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;

	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	this->front = glm::normalize(front);

	right = glm::normalize(glm::cross(this->front, worldUp));
	up = glm::normalize(glm::cross(right, this->front));
}
