#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"

class FrameBuffer
{
private:
	GLFWwindow* m_window;
	unsigned int m_FrameBuffer;
	unsigned int m_Texture;
public:
	FrameBuffer(GLFWwindow* window);
	~FrameBuffer();

	void renderToTexture();
	void renderToScreen();
};
