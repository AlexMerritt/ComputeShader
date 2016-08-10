#pragma once

#include "Window.h"
#include "GraphicDevice.h"
#include "RenderObject.h"
#include "Shader.h"
#include "Buffer.h"
#include "Camera.h"
#include "Timer.h"

class Renderer
{
public:

	bool Initialize(Window* window);

	GraphicDevice* GetGraphicDevice() { return m_graphicDevice; }

	void CreateField();
	RenderObject* CreateRenderObject(void* verticies, int numVerts, void* inds, int numInds, std::string vertexShader, std::string fragmentShader);

	void Update();
	void Render();

	void Render(RenderObject* renderObject);

private:
	void BeginScene();
	void EndScene();

private:
	GraphicDevice* m_graphicDevice;
	RenderObject* m_renderObject;
	Camera* m_camera;
	Timer* m_timer;

	std::vector<RenderObject*> m_objs;
};