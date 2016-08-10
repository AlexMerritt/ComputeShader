#include "System.h"


bool System::Initialize()
{
	bool result;

	Log::Info("Initializing systems");

	m_window = new Window();

	result = m_window->Initialize(1280, 720);
	if (!result)
	{
		Log::Error("Unable to create the window");
		return false;
	}

	m_renderer = new Renderer();

	result = m_renderer->Initialize(m_window);
	if (!result)
	{
		Log::Error("Unable to create the renderer");
		return false;
	}

	m_computeSystem = new ComputeSystem();

	result = m_computeSystem->Initialize(m_renderer->GetGraphicDevice());
	if (!result)
	{
		Log::Error("Unable to create the compute system");
	}

	m_particleSystem = new ParticleSystem();

	m_particleSystem->Initialize(m_computeSystem, m_renderer);

	Log::Info("Systems Initialized");

	//m_model = new Model();
	//m_model->Initialize(m_computeSystem);

	return true;
}

void System::Shutdown()
{
	Log::Info("Shutting down the system");
	delete m_window;

	Log::Info("System shutdown");
}

void System::Run()
{
	MSG msg;

	while (1) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if (msg.message == WM_QUIT)
		{
			break;
		}
		else
		{
			Update();
			Render();
		}
	}
}

void System::Update()
{
	m_particleSystem->Update(0.0f);
	m_renderer->Update();
}

void System::Render()
{
	m_renderer->Render();
}
