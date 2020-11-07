#pragma once

#include<chrono>
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "FrameBuffer.h"

#define GRIDRC 4 //Rendering grid # of rows and columns

class Fractal
{
public:
	Fractal(GLFWwindow* window);
	~Fractal();

	void OnUpdate();
	void OnRender();
	void OnImGuiRender();
	inline bool showGUI() const { return m_showGui; }
	inline bool doneRendering() const { return m_doneRendering; }
private:
	GLFWwindow* m_window;
	Shader m_Shaders[5];
	int m_currentShader; //Should be value 1 though 4, for indexes of m_Shaders. 
						 //Index 0 is the crosshair shader, and is always drawn. 
	Renderer m_Renderer;
	float m_positions[4 * (GRIDRC + 1) * (GRIDRC + 1)];
	unsigned int m_indices[6 * GRIDRC * GRIDRC];
	VertexArray m_va;
	VertexBuffer m_vb;
	VertexBufferLayout m_layout;
	IndexBuffer m_ib;
	FrameBuffer m_fb;
	glm::vec3 m_scale;
	glm::mat4 m_proj;
	glm::vec2 m_crosshair;
	glm::vec2 m_crosshairMandel;
	glm::vec2 m_offset;
	glm::vec2 m_offsetMandel;
	float m_zoom;
	float m_zoomMandel;
	float m_Exp;         //For caclulating our fractal -- n in z^n + c
	bool m_showGui;
	bool m_renderJulia;
	bool m_renderToTexture;
	long long m_renderTime;
	int m_scaleFactor;   //For scaling window coordinates and dimensions to smaller textures
	int m_maxIter;       //Increases by a factor of 2 up to maxIterMax every time we finish drawing a frame
	int m_maxIterMax;
	int m_windowWidth;
	int m_windowHeight;
	bool m_stop;         //Stop rendering if a key is pressed
	bool m_imgChanged;   //If the keypress registered changed the image, scrap the image and start over
	bool m_doneRendering;//True if we have rendered all frames we need to, up to full res scale and maxIterMax

	//generateBuffers generates a grid of (rows * cols) tiles to render to. We will check for user input after each
	//tile is rendered -- more tiles is more responsive to input but ultimately slower as it's checking more often
	void generateBuffers(float* buffer, unsigned int* indexes, int rows, int cols, float screenWidth, float screenHeight);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
