#pragma once
#include <vector>
#include "ComputeSystem.h"

#define NUM_DEGREE 360

struct Level
{
	// Array of floats
	float rads[NUM_DEGREE];
};

class Model
{
public:
	void Initialize(ComputeSystem* computeSystem);

	void Create(int numLevels);

	void AddLevel(Level level) { m_levels.push_back(level); }

	// Modifications
	void Widen(float size);

	void BuildVerticies();

private:
	void LoadComputeShaders();

private:
	float* GetDataRaw() { return (float*)m_levels.data(); }
	
	void PrintRaw();

	// x levels
	std::vector<Level> m_levels;
	ComputeSystem* m_computeSystem;

	ComputeShader* m_widenShader;
};

