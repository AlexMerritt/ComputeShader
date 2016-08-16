#include "Renderer.h"
#include "FileLoader.h"
#include "Util.h"
#include "Vector4.h"

bool Renderer::Initialize(Window * window)
{
	bool result;

	m_graphicDevice = new GraphicDevice();

	result = m_graphicDevice->Initialize(window);
	if (!result)
	{
		return false;
	}

	m_camera = new Camera();
	m_camera->Initialize(90.0f, (float)window->Width() / window->Height(), 0.1f, 1000.0f);
	m_camera->Update();

	m_renderObject = new RenderObject();

	m_timer = new Timer();
	m_timer->Initialize();

	m_elapsedTime = 0.0f;

	return true;
}	

RenderObject * Renderer::CreateRenderObject(void * verticies, int numVerts, void * inds, int numInds, std::string vertexShader, std::string fragmentShader)
{
	RenderObject* ro = new RenderObject();
	BufferData* vbd = new BufferData(verticies, numVerts, sizeof(Vector4));
	BufferData* ibd = new BufferData(inds, numInds, sizeof(unsigned int));

	ro->m_vertexBuffer = m_graphicDevice->CreateVertexBuffer(vbd);
	ro->m_indexBuffer = m_graphicDevice->CreateIndexBuffer(ibd);
	ro->m_numElements = numInds;
	ro->m_shader = m_graphicDevice->CreateShader(vertexShader, fragmentShader);

	return ro;
}

void Renderer::Update(float dt)
{

	//m_timer->Update();

	//m_camera->Move(Vector3(0, 0, 0.01));
	//m_camera->Update();

	//Log::Info("Elapsed Time: " + std::to_string(m_timer->GetTime()));
	m_elapsedTime += dt;
	float rad = 2;
	float angle = m_elapsedTime * 0.017f;

	m_camera->SetPosition(Vector3(sin(angle) * rad, 0.75f, cos(angle) * rad));
	m_camera->SetRotation(Vector3(sin(angle), 0.75f, cos(angle)));
	m_camera->Update();

}

void Renderer::Render()
{
	BeginScene();

	//m_graphicDevice->Render(m_camera, m_renderObject);

	for (unsigned int i = 0; i < m_objs.size(); ++i)
	{
		m_graphicDevice->Render(m_camera, m_objs[i]);
	}

	EndScene();
}

void Renderer::Render(RenderObject * renderObject)
{
	//m_graphicDevice->Render(m_camera, renderObject);
	m_objs.push_back(renderObject);
}

void Renderer::BeginScene()
{
	m_graphicDevice->BeginScene(0.125f, 0.125f, 0.125f);
}

void Renderer::EndScene()
{
	m_graphicDevice->EndScene();

	m_objs.clear();
}
