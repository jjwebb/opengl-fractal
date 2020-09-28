#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
//#include "Renderer.h"

//class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
	VertexBuffer* m_vb;
	VertexBufferLayout* m_layout;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer* vb, VertexBufferLayout* layout);

	void Bind();
	void Unbind();
};
