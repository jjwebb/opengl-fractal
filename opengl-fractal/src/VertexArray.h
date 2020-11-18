#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

/*Vertex array object -- links a vertex buffer and its appropriate layout
  so we don't have to set the layout for the buffer each time we render*/
class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};