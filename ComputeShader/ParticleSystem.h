#pragma once

#include <vector>

#include "ComputeSystem.h"
#include "Renderer.h"

#include "Vector4.h"
#include "Buffer.h"


struct ParticleEffect
{
	ParticleEffect() {}
	ParticleEffect(int numParticles, Vector4* positions, Vector4* speeds) : 
	numParticles(numParticles), positions(positions), speeds(speeds) {}
	int numParticles;
	Vector4* positions;
	Vector4* speeds;

	// The Particle system will fill these in
	Buffer positionBuffer;
	Buffer speedBuffer;
	RenderObject* renderObject;
};

class ParticleSystem
{
public:

	void Initialize(ComputeSystem* computeSystem, Renderer* renderer);

	void CreateEffect(int numParticles);

	void InitializeEffect(ParticleEffect* pe);

	void Update(float dt);

	void UpdateEffect(ParticleEffect* pe);

	void UpdateParticlePosition(ParticleEffect* pe);
	void BuildVerticies(ParticleEffect* pe);

private:
	std::vector<ParticleEffect> m_effects;

	ComputeShader* m_buildVerts;
	ComputeShader* m_updateParticles;

	ComputeSystem* m_computeSystem;
	Renderer* m_renderer;
};

