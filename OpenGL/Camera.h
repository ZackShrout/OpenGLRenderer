#pragma once

#include "Common.h"

class Camera
{
public:
	Camera() : m_yaw{ 0 }, m_pitch{ 0 }, m_rotation{ 0 }, m_movementSpeed{ 0 }, m_turnSpeed{ 0 } {}
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera() {}

	void KeyControl(bool* keys, GLfloat dt);
	void MouseControl(GLfloat xChange, GLfloat yChange);
	glm::mat4 CalculateViewMatrix() { return glm::lookAt(m_position, m_position + m_front, m_up); }
	glm::vec3 GetCameraPosition() { return m_position; }

private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;
	GLfloat m_yaw;
	GLfloat m_pitch;
	GLfloat m_rotation;
	GLfloat m_movementSpeed;
	GLfloat m_turnSpeed;

	void Update();
};

