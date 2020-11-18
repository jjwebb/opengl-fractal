#pragma once

//Holds our primitive vertex screen coordinates to be rendered on the gpu
class VertexBuffer
{
private: 
	unsigned int m_RendererID;
public: 
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void init(const void* data, unsigned int size); //Send the buffer to the GPU
};