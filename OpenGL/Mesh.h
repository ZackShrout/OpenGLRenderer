#pragma once

#include "Common.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh(float* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

private:
	GLuint m_VAO, m_VBO, m_IBO;
	GLsizei m_indexCount;
};

