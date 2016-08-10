#pragma once
#include "Buffer.h"
#include "Shader.h"

class Renderer;

struct VertexType
{
	float x, y, z, w;
};

class RenderObject
{
public:
	VertexBuffer GetVertexBuffer() { return m_vertexBuffer; }
	Buffer GetIndexBuffer() { return m_indexBuffer; }
	unsigned int GetElementCount() { return m_numElements; }
	Shader* GetShader() { return m_shader; }

private:
	VertexBuffer m_vertexBuffer;
	Buffer m_indexBuffer;
	unsigned int m_numElements;
	Shader* m_shader;

	friend Renderer;
};