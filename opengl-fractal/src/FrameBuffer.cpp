#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(GLFWwindow* window)
	: m_window(window)
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	m_Width = width;
	m_Height = height;

	//Create three framebuffers with three different texture sizes to be rendered to
	//Textures are scaled down by a factor of 4 (2x2) and 9 (3x3), which should reduce render time
	for (int i = 0; i < 3; i++)
	{
		GLCall(glGenFramebuffers(1, &m_FrameBuffer[i]));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer[i]));
		GLCall(glGenTextures(1, &m_Texture[i]));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture[i]));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width / (i+1), height / (i+1), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture[i], 0));
		GLCall(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	}
	GLCall(glViewport(0, 0, width, height));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));//switch back to rendering to screen
}

FrameBuffer::~FrameBuffer()
{
	for (int i = 0; i < 3; i++)
	{
		GLCall(glDeleteTextures(1, &m_Texture[i]));
		GLCall(glDeleteFramebuffers(1, &m_FrameBuffer[i]));
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

//Resize our textures when the window gets resized
void FrameBuffer::resizeTextures(int width, int height)
{
	m_Width = width; 
	m_Height = height;

	for (int i = 0; i < 3; i++)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture[i]));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width / (i + 1), height / (i + 1), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));

		GLCall(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	}
}

//Render to the texture of the specified scale
void FrameBuffer::renderToTexture(int scaleFactor)
{
	if (scaleFactor > 3)
		scaleFactor = 3;
	else if (scaleFactor < 1)
		scaleFactor = 1;

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer[scaleFactor-1]));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture[scaleFactor-1]));

	GLCall(glViewport(0, 0, m_Width / scaleFactor, m_Height / scaleFactor));
}

//Render to the visible framebuffer (the screen)
void FrameBuffer::renderToScreen()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	GLCall(glViewport(0, 0, m_Width, m_Height));
}
