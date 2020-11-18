#pragma once 

#include <GL/glew.h>
#include<GLFW/glfw3.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) assert(x);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
private:
    mutable int m_maxIterLast;
    mutable int m_scaleLast;
    mutable int m_iLast;
public:
    Renderer();
    ~Renderer();
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, 
    const bool& stop, const int& scale, const int& maxIter, const bool& changed) const;
};
