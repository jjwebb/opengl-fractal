#pragma once

//Holds a list of vertex indices from the vertex buffer to be rendered
class IndexBuffer
{
private: 
	unsigned int m_RendererID;
	unsigned int m_Count;
public: 
	IndexBuffer();
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	void init(const unsigned int* data, unsigned int count); //Send buffer to the GPU

	inline unsigned int GetCount() const { return m_Count; }
};