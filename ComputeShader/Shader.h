#pragma once

#include <vector>
#include "Buffer.h"

class Shader
{
public:
	Shader(unsigned int shader) : m_shader(shader) {}

	const unsigned int GetShader() { return m_shader; }

protected:
	unsigned int m_shader;
};

class ComputeShader : public Shader
{
public:
	ComputeShader(unsigned int shader, int numGroupX, int numGroupY, int numGroupZ) : Shader(shader), 
		m_numGroupX(numGroupX), m_numGroupY(numGroupY), m_numGroupZ(numGroupZ),
		m_groupSizeX(1), m_groupSizeY(1), m_groupSizeZ(1) {}

	void AddBuffer(Buffer b) { m_buffers.push_back(b); }
	const std::vector<Buffer> GetBuffers() { return m_buffers; }
	const Buffer GetBuffer(int i) { return m_buffers[i]; }

	void GetComputeSize(int& x, int& y, int& z) { x = m_numGroupX; y = m_numGroupY; z = m_numGroupZ; }
	void SetComputeSize(int x, int y, int z) { m_numGroupX = x; m_numGroupY = y; m_numGroupZ = z; }

	void GetGroupSize(int &x, int& y, int& z) { x = m_groupSizeX; y = m_groupSizeY; z = m_groupSizeZ; }
	void SetGroupSize(int x, int y, int z) { m_groupSizeX = x; m_groupSizeY = y; m_groupSizeZ = z; }

	void ClearBuffers() { m_buffers.clear(); }

private:
	std::vector<Buffer> m_buffers;
	int m_numGroupX;
	int m_numGroupY;
	int m_numGroupZ;
	int m_groupSizeX;
	int m_groupSizeY;
	int m_groupSizeZ;

};