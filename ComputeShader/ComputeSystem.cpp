#include "ComputeSystem.h"


bool ComputeSystem::Initialize(GraphicDevice * graphicDevice)
{
	m_graphicDevice = graphicDevice;

	return true;
}

ComputeShader * ComputeSystem::CreateShader(std::string src)
{
	ComputeShader* s = new ComputeShader(
		m_graphicDevice->CreateComputeShader(LoadFile(RESOURCE_PATH + src)), 0, 0, 0);

	return s;
}

Buffer ComputeSystem::CreateBuffer(void * data, unsigned int numElements, unsigned int elementSize)
{
	Buffer output;

	BufferData* bufferData = new BufferData(data, numElements, elementSize);

	output = CreateBuffer(bufferData);

	delete bufferData;

	return output;
}

Buffer ComputeSystem::CreateBuffer(BufferData * bd)
{
	return m_graphicDevice->CreateSBO(bd);
}

void ComputeSystem::SetFloatAttrib(Shader * shader, std::string param, float * data, int size)
{
	m_graphicDevice->SetUniformFloat(shader, param, data, size);
}

void ComputeSystem::ReleaseShader(ComputeShader * shader)
{
	m_graphicDevice->Delete(shader);
}

void ComputeSystem::ReleaseBuffer(Buffer * buffer)
{
	m_graphicDevice->Delete(*buffer);
}


void ComputeSystem::DispatchCompute(ComputeShader * shader)
{
	m_graphicDevice->DispatchShader(shader);
}
