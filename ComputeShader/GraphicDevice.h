#pragma once

#include "Window.h"
#include "RenderObject.h"
#include "Buffer.h"
#include "Shader.h"
#include "Matrix.h"

#include "Camera.h"

#include "glew.h"
#include "wglew.h"

#include <string>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

class GraphicDevice
{
public:
	bool Initialize(Window* window);

	GLuint CreateComputeShader(const std::string src);
	Shader* CreateShader(const std::string vs, const std::string fs);

	VertexBuffer CreateVertexBuffer(BufferData* data);
	Buffer CreateIndexBuffer(BufferData* data);
	Buffer CreateSBO(BufferData* data);

	void Delete(Buffer buffer);
	void Delete(Shader* shader);

	void* GetBufferData(Buffer bufferData);

	void Render(Camera* camera, RenderObject* renderObject);

	void BeginScene(float red, float green, float blue);
	void EndScene();

	void DispatchShader(ComputeShader* shader);

	void SetUniformFloat(Shader* shader, std::string param, float * data, unsigned int size);
	void SetUniformMatrix(Shader* shader, std::string param, Matrix4 m);

private:
	HDC m_deviceContext;
	HGLRC m_renderingContext;
};