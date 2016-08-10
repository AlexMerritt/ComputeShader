#include "ParticleSystem.h"

#include "Util.h"
#include "FileLoader.h"

void ParticleSystem::Initialize(ComputeSystem * computeSystem, Renderer* renderer)
{
	m_computeSystem = computeSystem;
	m_renderer = renderer;

	m_buildVerts = m_computeSystem->CreateShader("createVerts.cs");
	m_updateParticles = m_computeSystem->CreateShader("updateParticles.cs");

	CreateEffect(10000);
}

void ParticleSystem::CreateEffect(int numParticles)
{
	ParticleEffect pe;
	Vector4* positions;
	Vector4* speeds;
	

	positions = new Vector4[numParticles];
	speeds = new Vector4[numParticles];

	for (int i = 0; i < numParticles; ++i)
	{
		float angle = ((float)rand() / RAND_MAX) * 360;
		float speed = 0.01f; (float)rand() / RAND_MAX;

		positions[i] = Vector4(0, 0, 0);
		speeds[i] = Vector4(cos(angle) * speed, sin(angle) * speed, 0);
	}

	pe = ParticleEffect(numParticles, positions, speeds);

	InitializeEffect(&pe);

	m_effects.push_back(pe);

}

void ParticleSystem::InitializeEffect(ParticleEffect* pe)
{ 
	int numVerts = pe->numParticles * 4;
	int numInds = pe->numParticles * 6;

	pe->positionBuffer = m_computeSystem->CreateBuffer(pe->positions, pe->numParticles, sizeof(Vector4));
	pe->speedBuffer = m_computeSystem->CreateBuffer(pe->speeds, pe->numParticles, sizeof(Vector4));
	//pe->renderObject = m_renderer->CreateRenderObject(nullptr, numVerts, nullptr, numInds, "vs.vs", "fs.fs");
}

void ParticleSystem::Update(float dt)
{
	for (unsigned int i = 0; i < m_effects.size(); ++i)
	{
		UpdateEffect(&m_effects[i]);
		m_renderer->Render(m_effects[i].renderObject);
	}
}

void ParticleSystem::UpdateEffect(ParticleEffect * pe)
{
	UpdateParticlePosition(pe);

	BuildVerticies(pe);
}

void ParticleSystem::UpdateParticlePosition(ParticleEffect * pe)
{
	m_updateParticles->SetComputeSize(pe->numParticles, 1, 1);
	m_updateParticles->SetGroupSize(64, 1, 1);

	m_updateParticles->ClearBuffers();
	m_updateParticles->AddBuffer(pe->positionBuffer);
	m_updateParticles->AddBuffer(pe->speedBuffer);

	m_computeSystem->DispatchCompute(m_updateParticles);
}

void ParticleSystem::BuildVerticies(ParticleEffect * pe)
{
	Buffer vertBuffer, indBuffer;
	unsigned int numVerts = pe->numParticles * 4;
	unsigned int numInds = pe->numParticles * 6;

	vertBuffer = m_computeSystem->CreateBuffer(nullptr, numVerts, sizeof(Vector4));
	indBuffer = m_computeSystem->CreateBuffer(nullptr, numInds, sizeof(unsigned int));

	m_buildVerts->SetComputeSize(pe->numParticles, 1, 1);
	m_buildVerts->SetGroupSize(64, 1, 1);

	m_buildVerts->ClearBuffers();

	m_buildVerts->AddBuffer(pe->positionBuffer);
	m_buildVerts->AddBuffer(vertBuffer);
	m_buildVerts->AddBuffer(indBuffer);

	m_computeSystem->DispatchCompute(m_buildVerts);

	// Printing
	Buffer vb = m_buildVerts->GetBuffer(1);
	Vector4* vd = m_computeSystem->GetValues<Vector4>(vb);

	Buffer ib = m_buildVerts->GetBuffer(2);
	unsigned int* id = m_computeSystem->GetValues<unsigned int>(ib);

	/*for (unsigned int i = 0; i < numVerts; ++i)
	{
		Log::Info("Index[" + std::to_string(i) + "] " + std::to_string(vd[i].x) + ", " + std::to_string(vd[i].y) + ", " + std::to_string(vd[i].z));
	}*/

	/*for (unsigned int i = 0; i < numInds; ++i)
	{
		Log::Info("Index[" + std::to_string(i) + "] : " + std::to_string(id[i]));
	}*/

	pe->renderObject = m_renderer->CreateRenderObject(vd, numVerts, id, numInds, LoadFile(RESOURCE_PATH + "vs.vs"), LoadFile(RESOURCE_PATH + "fs.fs"));

	delete id;
	delete vd;

	m_computeSystem->ReleaseBuffer(&vertBuffer);
	m_computeSystem->ReleaseBuffer(&indBuffer);
}
