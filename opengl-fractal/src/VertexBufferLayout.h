#pragma once

#include<GL/glew.h>
#include<vector>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;//size 4 bytes
			case GL_UNSIGNED_INT:	return 4;//size 4 bytes
			case GL_UNSIGNED_BYTE:	return 1;//size 1 byte
		}
		ASSERT(false);//Throw an error if the type given wasn't in the list
		return 0;
	}
};

/*This class specifes the layout for the vertex buffer -- the stride (offset) in bytes 
for each element in the array. In this fractal project we have a stride of 16 -- two
4-byte floats representing primitive vertex coordinates and another two for 
texture mapping coordinates*/ 
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count);

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};
