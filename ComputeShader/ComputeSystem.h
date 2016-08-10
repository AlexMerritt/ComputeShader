#pragma once
#include "GraphicDevice.h"
#include "Buffer.h"
#include "Shader.h"
#include "FileLoader.h"

class ComputeSystem
{
public:
	bool Initialize(GraphicDevice* graphicDevice);

	ComputeShader* CreateShader(std::string src);

	// Might want to tie buffers to a shader
	Buffer CreateBuffer(void* data, unsigned int numElements, unsigned int elementSize);
	Buffer CreateBuffer(BufferData* bd);

	void DispatchCompute(ComputeShader* shader);

	template<typename t>
	inline t* GetValues(Buffer buffer) { return (t*)m_graphicDevice->GetBufferData(buffer); }

	void SetFloatAttrib(Shader* shader, std::string param, float* data, int size);

	//void* GetValues(Buffer buffer);

	void ReleaseShader(ComputeShader* shader);
	void ReleaseBuffer(Buffer* buffer);

private:
	GraphicDevice* m_graphicDevice;
};