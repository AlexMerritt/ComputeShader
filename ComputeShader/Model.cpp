#include "Model.h"
#include <string>
#include "Util.h"
#include "Math.h"
#include <time.h>

#include "Timer.h"

void Model::Initialize(ComputeSystem * computeSystem)
{
	m_computeSystem = computeSystem;

	LoadComputeShaders();

	Timer* t = new Timer();
	t->Initialize();

	float time;

	
	
	Create(20000);
	//PrintRaw();
	t->Update();
	
	Widen(1.5f);

	
	t->Update();
	time = t->GetTime();

	//PrintRaw();

	Log::Info("Elapsed time: " + std::to_string(time));
}

void Model::Create(int numLevels)
{
	unsigned int numValues = numLevels * NUM_DEGREE;
	float max = 10.0f;
	float min = 1.0f;
	for (int i = 0; i < numLevels; ++i)
	{
		Level l;

		for (unsigned int j = 0; j < NUM_DEGREE; ++j)
		{
			l.rads[j] = (float)j;// min + (((float)rand() / RAND_MAX) * (max - min));
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
	ComputeShader* shader;
	Buffer modelBuffer;

	modelBuffer = m_computeSystem->CreateBuffer(m_levels.data(), numItems, sizeof(float));
	
	shader = m_computeSystem->CreateShader("modelbuild.cs");
}

void Model::LoadComputeShaders()
{
	m_widenShader = m_computeSystem->CreateShader("widen.cs");
	m_widenShader->SetComputeSize(NUM_DEGREE, m_levels.size(), 1); // Num of computes
	m_widenShader->SetGroupSize(32, 32, 1); // Num Threads running at a time
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
