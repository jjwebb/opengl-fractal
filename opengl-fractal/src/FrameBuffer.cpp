#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(GLFWwindow* window)
	: m_window(window)
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);

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
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));//switch back to rendering to screen
}

FrameBuffer::~FrameBuffer()
{
	GLCall(glDeleteFramebuffers(1, &m_FrameBuffer[1]));
	GLCall(glDeleteTextures(1, &m_Texture[1]));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::renderToTexture(int scaleFactor)
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer[scaleFactor-1]));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture[scaleFactor-1]));
	//GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture));
	
	GLCall(glViewport(0, 0, width, height));
}

void FrameBuffer::renderToScreen()
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	//GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture));
	GLCall(glViewport(0, 0, width, height));
}
