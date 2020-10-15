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

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    int quads = ib.GetCount() / 6;
    shader.Bind();
    va.Bind();
    ib.Bind(); //va does this already (does it????)
    for (int i = 0; i < quads; i++)
    {
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int))));
    }
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const bool& stop) const
{
    int quads = ib.GetCount() / 6;
    shader.Bind();
    va.Bind();
    ib.Bind(); //va does this already (does it????)
    for (int i = 0; i < quads; i++)
    {
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int))));
        glfwPollEvents();
        if (stop)
        {
            std::cout << "Rendered " << i + 1 << " quads before returning" << std::endl;
            return;
        }
    }
}