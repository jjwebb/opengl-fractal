#include "Renderer.h"
#include <iostream>
void GLClearError()
{
    while (glGetError());
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " in file " << file << " line " << line << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer()
{
    m_iLast = 0;
    m_maxIterLast = 0;
    m_scaleLast = 0;
}

Renderer::~Renderer()
{
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    Clear();
    int quads = ib.GetCount() / 6;
    shader.Bind();
    va.Bind();
    ib.Bind();
    for (int i = 0; i < quads; i++)
    {
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int))));
    }
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, 
const bool& stop, const int& scale, const int& maxIter, const bool& changed) const
{
    int quads = ib.GetCount() / 6;
    shader.Bind();
    va.Bind();
    ib.Bind();
    if (scale != m_scaleLast || maxIter != m_maxIterLast)
    {
        Clear();
        m_iLast = 0;
    }

    for (int i = m_iLast; i < quads; i++)
    {
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int))));
        GLCall(glFinish());
        glfwPollEvents();

        if (stop && (scale != 3 || changed))
        {
            m_iLast = !changed ? i : 0;
            m_scaleLast = scale;
            m_maxIterLast = maxIter;
            return;
        }
    }
    m_scaleLast = scale;
    m_maxIterLast = maxIter;
}
