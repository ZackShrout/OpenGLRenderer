#pragma once

#include "Common.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera();

	void KeyControl(bool* keys, GLfloat dt);
	void MouseControl(GLfloat xChange, GLfloat yChange);
	glm::mat4 CalculateViewMatrix();

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

