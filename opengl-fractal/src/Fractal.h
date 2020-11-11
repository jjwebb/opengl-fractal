#pragma once

#include<chrono>
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "FrameBuffer.h"

//With a X/Y ratio of 5.0/2.8125 (16:9) the center of the screen is at
//C coords (2.501302, 1.407552). We have to factor this into our equation for 
//the zoom to scale properly
#define XMUL 5.0f
#define YMUL 2.8125f
#define XSUBT 2.501302f
#define YSUBT 1.407552f

#define GRIDRC 4 //Rendering grid # of rows and columns

class Fractal
{
public:
	Fractal(GLFWwindow* window);
	~Fractal();

	void MainRenderLoop();//Main logic loop - prepare to render on the GPU
	void OnImGuiRender(); //GUI render logic
	inline bool showGUI() const { return m_showGui; }
	inline bool doneRendering() const { return m_doneRendering; }
private:
	GLFWwindow* m_window;
	Shader m_Shaders[5];
	int m_currentShader;   //Should be value 1 though 4, for indexes of m_Shaders. 
						   //Index 0 is the crosshair shader, and is always drawn. 
	Renderer m_Renderer;
	VertexArray m_va;      //Vertex Array Objext -- links vertex buffer and its layout
	VertexBuffer m_vb;     //Stores primitie vertex coordinates
	VertexBufferLayout m_layout;//Specifies the number of bytes between entities 
								//in the vertex buffer
	IndexBuffer m_ib;      //List of indexes from the vertex buffer to render
	FrameBuffer m_fb;      //Holds our textures of various scales
	glm::vec3 m_scale;
	glm::mat4 m_proj;
	glm::vec2 m_crosshair;
	glm::vec2 m_crosshairMandel;
	glm::vec2 m_offset;
	glm::vec2 m_offsetMandel;
	float m_zoom;
	float m_zoomMandel;    //The Mandel variables are for keeping track of the Mandelbrot state in Julia mode
	float m_Exp;           //For caclulating our fractal -- n in z^n + c
	bool m_showGui;
	bool m_renderJulia;
	bool m_renderToTexture;//When false, we draw the crosshair over the pre-rendered texture
	long long m_renderTime;
	int m_scaleFactor;     //For scaling window coordinates and dimensions to smaller textures
	int m_maxIter;         //Increases by a factor of 2 up to maxIterMax every time we finish drawing a frame
	int m_maxIterMax;
	int m_windowWidth;
	int m_windowHeight;
	bool m_stop;           //Stop rendering if a key is pressed
	bool m_imgChanged;     //If the keypress registered changed the image, scrap the image and start over
	bool m_doneRendering;  //True if we have rendered all frames we need to, up to full res scale and maxIterMax

	/*generateBuffers generates a grid of (rows * cols) tiles to render to. We will check for user input after each
	  tile is rendered -- more tiles is more responsive to input but ultimately slower as it's checking more often*/
	void generateBuffers(float* buffer, unsigned int* indexes, int rows, int cols, float screenWidth, float screenHeight);
	
	void Render();      //Render on the GPU

	//These functions return the Z or C coordinate pointed to by the crosshair rather than raw screen coords
	inline float crosshairCoordX() const {
		return ((m_crosshair.x / m_windowWidth ) * XMUL - XSUBT) * m_zoom + m_offset.x; }
	inline float crosshairCoordY() const {
		return ((m_crosshair.y / m_windowHeight) * YMUL - YSUBT) * m_zoom + m_offset.y; }
	inline float crosshairMandelCoordX() const {
		return ((m_crosshairMandel.x / m_windowWidth) * XMUL - XSUBT) * m_zoomMandel + m_offsetMandel.x; }
	inline float crosshairMandelCoordY() const {
		return ((m_crosshairMandel.y / m_windowHeight) * YMUL - YSUBT) * m_zoomMandel + m_offsetMandel.y; }

	inline void resetCrosshair() {
		m_crosshair.x = m_windowWidth / 2 + 0.5f;
		m_crosshair.y = m_windowHeight / 2 + 0.5f;
	}
	inline void resetCrosshairMandel() {
		m_crosshairMandel.x = m_windowWidth / 2 + 0.5f;
		m_crosshairMandel.y = m_windowHeight / 2 + 0.5f;
	}
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
