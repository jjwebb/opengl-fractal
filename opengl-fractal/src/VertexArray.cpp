#include "VertexArray.h"

//#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
	: m_vb(NULL),
	m_layout(NULL)
{
	//GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	//GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(VertexBuffer* vb, VertexBufferLayout* layout)
{
	if(m_vb == NULL || m_layout == NULL)
	{
		m_vb = vb;
		m_layout = layout;
	}
	//Bind();
	vb->Bind();
	const auto& elements = layout->GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout->GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind()
{
	AddBuffer(m_vb, m_layout);
	//GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind()
{
	//GLCall(glBindVertexArray(0));
}
