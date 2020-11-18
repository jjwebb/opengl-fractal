#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer()
{
    m_RendererID = 0; //This is the ID of our buffer on the GPU
    m_Count = 0; //Size of the buffer
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
    init(data, count);
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

//Create index buffer on the GPU and send data to it
void IndexBuffer::init(const unsigned int* data, unsigned int count)
{
    m_Count = count;
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}