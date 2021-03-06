#pragma once

#include <GL/glew.h>
#include<GLFW/glfw3.h>
#include "Renderer.h"

/*This class handles a FrameBuffer Object that stores our rendered textures of different sizes
  to sample back in the crosshair shader instead of calculating the set repeatedly*/
class FrameBuffer
{
private:
	GLFWwindow* m_window;
	unsigned int m_FrameBuffer[3];
	unsigned int m_Texture[3]; //Each texture is bound to its own framebuffer
	int m_Height;
	int m_Width;
public:
	FrameBuffer(GLFWwindow* window);
	~FrameBuffer();

	void resizeTextures(int width, int height); //Resize our textures when the window size changes
	void renderToTexture(int scaleFactor); //Draw the set into the texture
	void renderToScreen(); //Sample from the texture
};