#pragma once

#include "Common.h"

class Window
{
public:
	Window();
	~Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialize();
	int GetBufferWidth() { return m_bufferWidth; };
	int GetBufferHeight() { return m_bufferHeight; };
	bool GetShouldClose() { return glfwWindowShouldClose(m_mainWindow); };
	bool* GetKeys() { return m_keys; };
	float GetXChange();
	float GetYChange();
	void SwapBuffers() { glfwSwapBuffers(m_mainWindow); };

private:
	GLFWwindow* m_mainWindow;
	GLint m_width, m_height;
	GLint m_bufferWidth, m_bufferHeight;
	bool m_keys[1024];
	GLfloat m_lastX;
	GLfloat m_lastY;
	GLfloat m_xChange;
	GLfloat m_yChange;
	bool m_mouseFirstMoved;

	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
};

