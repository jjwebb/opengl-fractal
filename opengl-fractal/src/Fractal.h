#pragma once

#include<chrono>
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "FrameBuffer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/gtc/matrix_transform.hpp"

#define YRANGE 1.2f //Maximum i value displayed at program start

#define GRIDRC 4 //Rendering grid # of rows and columns

#define SHADER_CROSSHAIR 0 //Shader array index values
#define SHADER_MANDELBROT 1
#define SHADER_MULTIBROT 2
#define SHADER_JULIA 3
#define SHADER_MULTIJULIA 4

class Fractal
{
public:
	Fractal(GLFWwindow* window);
	~Fractal();

	void MainRenderLoop();//Main logic loop - prepare to render on the GPU

	void OnImGuiRender(); //GUI render logic

	inline bool showGUI() const { return m_showGui; }//Press I to toggle

	//True if m_maxIter >= m_maxIterMax and m_scaleFactor == 0
	inline bool doneRendering() const { return m_doneRendering; }

private:
	GLFWwindow* m_window;		//The program's window object
	Shader m_Shaders[5];		//Holds all our shader programs
	int m_currentShader;		//Should be value 1 though 4, for indexes of m_Shaders. 
								//Index 0 is the crosshair shader, and is always drawn. 
	Renderer m_Renderer;		//Send data to the GPU
	VertexArray m_va;			//Vertex Array Objext -- links vertex buffer and its layout
	VertexBuffer m_vb;			//Stores primitie vertex coordinates
	VertexBufferLayout m_layout;//Specifies the number of bytes between entities in the vertex buffer
	IndexBuffer m_ib;			//List of indexes from the vertex buffer to render
	FrameBuffer m_fb;			//Holds our textures of various scales
	glm::mat4 m_proj;				  //Projection matrix -- normalize Mandelbrot coordinates to (-1, 1)
	glm::mat4 m_projCrosshair;	      //Matrix to normalize crosshair coords to (-1, 1)
	glm::vec2 m_crosshair;            //This is the crosshair in view at all times, even in Julia mode
	glm::vec2 m_crosshairMandel;      //Crosshair used to determine the point C for a Julia set
	glm::vec2 m_crosshairMandelStatic;//Keeps track of original Mandel position to change back to with H key
	glm::vec2 m_offset;				  //The C value at the center of the screen
	glm::vec2 m_offsetMandel;	      //Used with crosshairMandel to get our C value in Julia set mode
	glm::vec2 m_offsetMandelStatic;   //Keeps track of original C to change back to with H key
	ImVec2 m_guiOffset;		       	  //Pixel offset from lower right hand corner of screen for GUI window
	float m_zoom;		   //A smaller number means a smaller difference between points, which means we zoom in
	float m_zoomMandel;    //The Mandel variables are for keeping track of the Mandelbrot state in Julia mode
	float m_Exp;           //For caclulating our fractal -- n in z^n + c
	float m_deltaExp;      //How much n changes by when we change it (.01, .05, 1)
	bool m_showGui;        //Press the i key to toggle coordinate information on/off
	bool m_showRenderTime; //Press T to toggle frame render time info (showGUI must be true)
	bool m_renderJulia;    //Press J/H to enter Julia set mode from the Mandelbrot set
	bool m_renderToTexture;//When false, we draw the crosshair over the pre-rendered texture
	long long m_renderTime;//How long it took to render the most recent frame
	int m_scaleFactor;     //For scaling window coordinates and dimensions to smaller textures
	int m_maxIter;         //Increases by a factor of 2 up to maxIterMax every time we finish drawing a frame
	int m_maxIterMax;	   //The maximum iteration threashold; after wich (at scale 1) we are done rendering
	int m_windowWidth;	   //X resolution of the screen in fullscreen
	int m_windowHeight;	   //Y resolution of the screen in fullscreen
	float m_aspectRatio;   //X resolution / Y resolution
	bool m_fullscreen;	   //Whether to display the program in fullscreen
	int  m_refreshRate;	   //Screen refresh rate -- lowering this will cause the image to change more slowly
	bool m_showCrosshair;  //Whether to display the crosshair on the screen
	bool m_stop;           //Stop rendering if a key is pressed
	bool m_imgChanged;     //If the keypress registered changed the image, scrap the image and start over
	bool m_doneRendering;  //True if we have rendered all frames we need to, up to full res scale and maxIterMax

	/*generateBuffers generates a grid of (rows * cols) tiles to render to. We will check for user input after each
	  tile is rendered -- more tiles is more responsive to input but ultimately slower as it's checking more often*/
	void generateBuffers(float* buffer, unsigned int* indexes, int rows, int cols, float screenWidth, float screenHeight);
	
	//Render on the GPU
	void Render();

	//Zoom in or out
	void zoom(bool zoomIn);

	//Returns the Z (if Julia) or C coordinates pointed to by the crosshair rather than raw screen coords
	inline glm::vec2 crosshairCoords() const {
		//Normalize crosshair coords to range of (-1.0, 1.0), (-1.0, 1.0)
		glm::vec4 translation = (m_projCrosshair * glm::vec4((m_crosshair.x - 0.5f), (m_crosshair.y - 0.5f), -1.0f, 1.0f));
		
		//X coord = normalized (-1.0, 1.0) x value * max Y val * aspect ratio * zoom + offset
		return glm::vec2(translation.x * YRANGE * (float(m_windowWidth) / m_windowHeight) * m_zoom + m_offset.x,
						 translation.y * YRANGE * m_zoom + m_offset.y);
	}

	//Returns the C coordinate pointed to by the crosshair on the Mandelbrot set when in Julia mode
	inline glm::vec2 crosshairMandelCoords() const {
		//Normalize crosshair coords to range of (-1.0, 1.0), (-1.0, 1.0)
		glm::vec4 translation = (m_projCrosshair * glm::vec4((m_crosshairMandel.x - 0.5f), (m_crosshairMandel.y - 0.5f), -1.0f, 1.0f));

		//X coord = normalized (-1.0, 1.0) x value * max Y val * aspect ratio * zoom + offset
		return glm::vec2(translation.x * YRANGE * (float(m_windowWidth) / m_windowHeight) * m_zoomMandel + m_offsetMandel.x,
						 translation.y * YRANGE * m_zoomMandel + m_offsetMandel.y);
	}

	//Toggles whether or not the crosshair will be displayed
	inline void toggleCrosshair() {
		m_showCrosshair = !m_showCrosshair;
		m_Shaders[0].SetUniform1i("u_showCrosshair", m_showCrosshair);
	}

	inline void checkCrosshair() {
		if (m_zoom != m_zoomMandel)
			m_showCrosshair = false;
		else
			m_showCrosshair = true;
	}

	/*Returns an amount for the crosshair to move relative to the difference between 
	  the zoom level on the Mandelbrot set and the zoom level of the Julia set displayed*/
	inline int crosshairZoomDiff() { 
		int diff = int(20 * (m_zoomMandel / m_zoom));
		if (diff == 0)
		{
			if (m_showCrosshair)
				toggleCrosshair();
		}
		else
		{
			if (!m_showCrosshair)
				toggleCrosshair();
		}
		return diff;
	}

	//Resets the crosshair in view
	inline void resetCrosshair() {
		m_crosshair.x = m_windowWidth / 2 + 0.5f;
		m_crosshair.y = m_windowHeight / 2 + 0.5f;
	}

	//Resets the crosshair point used to get the C value to generate a Julia set
	inline void resetCrosshairMandel() {
		m_crosshairMandel.x = m_windowWidth / 2 + 0.5f;
		m_crosshairMandel.y = m_windowHeight / 2 + 0.5f;
	}

	//Sets offset to crosshair position
	inline void setOffset() {
		glm::vec2 coords = crosshairCoords();
		m_offset.x = coords.x;
		m_offset.y = coords.y;
		m_Shaders[m_currentShader].SetUniform2f("u_offset", m_offset.x, m_offset.y);
	}

	/*Sets Mandel offset to crosshairMandel position. We don't send this to the GPU because
	  "offset" always represents the offset seen on the screen*/
	inline void setOffsetMandel() {
		glm::vec2 coords = crosshairMandelCoords();
		m_offsetMandel.x = coords.x;
		m_offsetMandel.y = coords.y;
	}

	//Sets offset to 0
	inline void resetOffset() {
		m_offset.x = 0.0f;
		m_offset.y = 0.0f;
		m_Shaders[m_currentShader].SetUniform2f("u_offset", m_offset.x, m_offset.y);
	}

	//Set the projection matrix to account for the current zoom level
	inline void setProjectionMatrix() {
		float xMax = m_aspectRatio * YRANGE;
		m_proj = glm::ortho(-xMax * m_zoom, xMax * m_zoom, -YRANGE * m_zoom, YRANGE * m_zoom, -1.0f, 1.0f);
		m_Shaders[m_currentShader].SetUniformMat4f("u_MVP", m_proj);
	}

	//Change the Julia set displayed if C changes
	inline void changeJulia() {
		glm::vec2 C = crosshairMandelCoords();
		m_Shaders[m_currentShader].SetUniform2f("u_cVals", C.x, C.y);
		resetRenderQuality();
	}

	//Reset render quality to minimum value
	inline void resetRenderQuality() {
		m_imgChanged = true;
		if(m_scaleFactor == 2)
		{
			if (m_renderTime > 30000)
				m_maxIter = m_maxIterMax / 4;
			else if (m_renderTime > 20000)
				m_maxIter = m_maxIterMax / 2;
			else
				m_maxIter = m_maxIterMax;
		}
		else if(m_scaleFactor == 3)
		  m_maxIter = m_maxIterMax / 4;
		m_scaleFactor = 3;
	}

	//Set the position of the ImGUI window
	inline void setGUIpos() { 
		ImGui::SetWindowPos("Mandelbrot", ImVec2(m_windowWidth - m_guiOffset.x, m_windowHeight - m_guiOffset.y)); 
	}

	//Handle all key events for the program
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	//Handle window resize events
	static void window_size_callback(GLFWwindow* window, int width, int height);
};
