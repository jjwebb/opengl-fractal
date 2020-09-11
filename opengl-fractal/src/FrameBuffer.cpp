#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(GLFWwindow* window)
	: m_window(window)
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);

	GLCall(glGenFramebuffers(1, &m_FrameBuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
	GLCall(glGenTextures(1, &m_Texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

	//GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_Texture, 0));//Texture2d?
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0));
	GLCall(glCheckFramebufferStatus(GL_FRAMEBUFFER));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));//switch back to rendering to screen
}

FrameBuffer::~FrameBuffer()
{
	GLCall(glDeleteFramebuffers(1, &m_FrameBuffer));
	GLCall(glDeleteTextures(1, &m_Texture));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::renderToTexture()
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
	//GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture));
	GLCall(glViewport(0, 0, width, height));

}

void FrameBuffer::renderToScreen()
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	//GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture));
	GLCall(glViewport(0, 0, width, height))
}
