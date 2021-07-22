#include "Camera.h"

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
	: m_position{ startPosition }, m_worldUp{ startUp }, m_yaw{ startYaw }, m_pitch{ startPitch }, m_rotation { 0 }, m_movementSpeed{ startMoveSpeed }, m_turnSpeed{ startTurnSpeed }
{
	m_front = glm::vec3(0.0f, 0.0f, 1.0f);

	Update();
}

void Camera::KeyControl(bool* keys, GLfloat dt)
{
	GLfloat velocity = m_movementSpeed * dt;
	
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		m_position += m_front * velocity;
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		m_position -= m_front * velocity;
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		m_position -= m_right * velocity;
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		m_position += m_right * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= m_turnSpeed;
	yChange *= m_turnSpeed;

	m_yaw += xChange;
	m_pitch += yChange;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}

	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	Update();
}

void Camera::Update()
{
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(m_front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
