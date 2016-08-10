#pragma once

#include <Windows.h>
#include "Util.h"
#include "Model.h"

#include "Window.h"
#include "Renderer.h"
#include "ComputeSystem.h"
#include "ParticleSystem.h"

class System
{
public:

	bool Initialize();

	void Shutdown();

	void Run();

private:
	void Update();
	void Render();

private:
	Window* m_window;
	Renderer* m_renderer;
	ComputeSystem* m_computeSystem;
	ParticleSystem* m_particleSystem;

	Model* m_model;
};

