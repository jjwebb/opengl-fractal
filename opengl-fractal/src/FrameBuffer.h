#pragma once

#include <GL/glew.h>
#include<GLFW/glfw3.h>
#include "Renderer.h"

class FrameBuffer
{
private:
	GLFWwindow* m_window;
	unsigned int m_FrameBuffer[3];
	unsigned int m_Texture[3];
public:
	FrameBuffer();
	FrameBuffer(GLFWwindow* window);
	~FrameBuffer();

	void renderToTexture(int scaleFactor);
	void renderToScreen();
};