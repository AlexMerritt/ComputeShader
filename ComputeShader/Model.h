#pragma once
#include <vector>
#include "ComputeSystem.h"
#include "Vector4.h"
#include "Renderer.h"

#define NUM_DEGREE 120

struct Level
{
	// Array of floats
	float rads[NUM_DEGREE];
};

class Model
{
public:
	void Initialize(ComputeSystem* computeSystem, Renderer* renderer);

	void Create(int numLevels);

	void AddLevel(Level level) { m_levels.push_back(level); }

	// Modifications
	void Widen(float size);

	void BuildVerticies();

	void GenerateToolPath();

	void Update();

private:
	void LoadComputeShaders();

private:
	float* GetDataRaw() { return (float*)m_levels.data(); }
	
	void PrintRaw();

private:
	ComputeSystem* m_computeSystem;
	Renderer* m_renderer;

	// x levels
	std::vector<Level> m_levels;
	

	ComputeShader* m_widenShader;
	ComputeShader* m_buildVertsShader;
	ComputeShader* m_generateToolPathShader;
	ComputeShader* m_buildToolPathVertsShader;

	RenderObject* m_toolPathRO;
};

