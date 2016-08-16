#include "Model.h"
#include <string>
#include "Util.h"
#include "Math.h"
#include <time.h>

#include "Timer.h"

void Model::Initialize(ComputeSystem * computeSystem, Renderer* renderer)
{
	m_computeSystem = computeSystem;
	m_renderer = renderer;

	LoadComputeShaders();

	Timer* t = new Timer();
	t->Initialize();

	float time;

	Create(20);
	//PrintRaw();
	t->Update();
	
	//Widen(200.5f);

	
	t->Update();
	time = t->GetTime();

	//PrintRaw();

	GenerateToolPath();

	Log::Info("Elapsed time: " + std::to_string(time));
}

void Model::Create(int numLevels)
{
	unsigned int numValues = numLevels * NUM_DEGREE;
	float max = 1.025f;
	float min = 1.0f;
	for (int i = 0; i < numLevels; ++i)
	{
		Level l;

		for (unsigned int j = 0; j < NUM_DEGREE; ++j)
		{
			//l.rads[j] = min + (((float)rand() / RAND_MAX) * (max - min));
			//l.rads[j] = (float)i / 25.0f;
			l.rads[j] = 1.0f;
		}

		AddLevel(l);
	}
}

void Model::Widen(float size)
{
	unsigned int numItems = m_levels.size() * NUM_DEGREE;
	Buffer modelBuffer;

	modelBuffer = m_computeSystem->CreateBuffer(m_levels.data(), numItems, sizeof(float));

	// We need to clear the buffers so that we can use them again
	m_widenShader->ClearBuffers();
	m_widenShader->AddBuffer(modelBuffer);

	m_computeSystem->SetFloatAttrib(m_widenShader, "inc", &size, 1);

	m_widenShader->SetComputeSize(NUM_DEGREE, m_levels.size(), 1); // Num of computes
	m_widenShader->SetGroupSize(32, 32, 1); // Num Threads running at a time


	m_computeSystem->DispatchCompute(m_widenShader);

	modelBuffer = m_widenShader->GetBuffer(0);
	float* data = m_computeSystem->GetValues<float>(modelBuffer);

	void* dst = m_levels.data();

	// Copy the results back to our array of levels
	memcpy(dst, data, sizeof(float) * numItems);

	// Release our data
	m_computeSystem->ReleaseBuffer(&modelBuffer);

	return;
}

void Model::BuildVerticies()
{
	unsigned int numItems = m_levels.size() * NUM_DEGREE;
	Buffer modelBuffer;

	modelBuffer = m_computeSystem->CreateBuffer(m_levels.data(), numItems, sizeof(float));
	
}

void Model::GenerateToolPath()
{
	unsigned int numItems = m_levels.size() * NUM_DEGREE;
	Buffer outputBuffer, modelBuffer;

	modelBuffer = m_computeSystem->CreateBuffer(m_levels.data(), numItems, sizeof(float));
	outputBuffer = m_computeSystem->CreateBuffer(nullptr, numItems, sizeof(Vector4));

	m_generateToolPathShader->ClearBuffers();
	m_generateToolPathShader->AddBuffer(modelBuffer);
	m_generateToolPathShader->AddBuffer(outputBuffer);

	float levelHeight = 0.1f;
	float numDegree = NUM_DEGREE;

	m_computeSystem->SetFloatAttrib(m_generateToolPathShader, "levelHeight", &levelHeight, 1);
	m_computeSystem->SetFloatAttrib(m_generateToolPathShader, "numDegree", &numDegree, 1);

	m_generateToolPathShader->SetComputeSize(NUM_DEGREE, m_levels.size(), 1);
	m_generateToolPathShader->SetGroupSize(1, 1, 1);

	m_computeSystem->DispatchCompute(m_generateToolPathShader);	

	outputBuffer = m_generateToolPathShader->GetBuffer(1);
	Vector4* data = m_computeSystem->GetValues<Vector4>(outputBuffer);

	/*for (unsigned int i = 0; i < numItems; ++i)
	{
		Log::Info("Index[" + std::to_string(i) + "] " + std::to_string(data[i].y));
	}*/

	Buffer vertexBuffer, indexBuffer;
	unsigned int numVerts = numItems * 4;
	unsigned int numInds = numItems * 6;

	vertexBuffer = m_computeSystem->CreateBuffer(nullptr, numVerts, sizeof(Vector4));
	indexBuffer = m_computeSystem->CreateBuffer(nullptr, numInds, sizeof(unsigned int));

	m_buildToolPathVertsShader->ClearBuffers();
	m_buildToolPathVertsShader->AddBuffer(outputBuffer);
	m_buildToolPathVertsShader->AddBuffer(vertexBuffer);
	m_buildToolPathVertsShader->AddBuffer(indexBuffer);

	m_buildToolPathVertsShader->SetComputeSize(NUM_DEGREE, m_levels.size(), 1);
	m_buildToolPathVertsShader->SetGroupSize(1, 1, 1);

	m_computeSystem->DispatchCompute(m_buildToolPathVertsShader);
	
	Buffer vb = m_buildToolPathVertsShader->GetBuffer(1);
	Vector4* vd = m_computeSystem->GetValues<Vector4>(vb);

	Buffer ib = m_buildToolPathVertsShader->GetBuffer(2);
	unsigned int* id = m_computeSystem->GetValues<unsigned int>(ib);

	/*for (unsigned int i = 0; i < numVerts; ++i)
	{
	Log::Info("Index[" + std::to_string(i) + "] " + std::to_string(vd[i].x) + ", " + std::to_string(vd[i].y) + ", " + std::to_string(vd[i].z));
	}*/

	m_toolPathRO = m_renderer->CreateRenderObject(vd, numVerts, id, numInds, LoadFile(RESOURCE_PATH + "vs.vs"), LoadFile(RESOURCE_PATH + "fs.fs"));

	m_computeSystem->ReleaseBuffer(&modelBuffer);
	m_computeSystem->ReleaseBuffer(&outputBuffer);
}

void Model::Update()
{
	m_renderer->Render(m_toolPathRO);
}

void Model::LoadComputeShaders()
{
	m_widenShader = m_computeSystem->CreateShader("widen.cs");
	
	/*m_buildVertsShader = m_computeSystem->CreateShader("modelbuild.cs");
	m_buildVertsShader->SetComputeSize(NUM_DEGREE, m_levels.size(), 1);
	m_buildVertsShader->SetGroupSize(32, 32, 1);*/

	m_generateToolPathShader = m_computeSystem->CreateShader("generateToolPath.cs");

	m_buildToolPathVertsShader = m_computeSystem->CreateShader("createToolPathVerts.cs");
}

void Model::PrintRaw()
{
	float* data = (float*)m_levels.data();

	Log::Info("----- Begin Model Print -----");

	for (unsigned int i = 0; i < (m_levels.size() * NUM_DEGREE); ++i)
	{
		Log::Info("Element[" + std::to_string(i) + "] : " + std::to_string(data[i]));
	}

	Log::Info("----- End Model Print -----");
}
