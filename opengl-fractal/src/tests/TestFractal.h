#pragma once

#include<chrono>
#include "Test.h"
#include "Texture.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "FrameBuffer.h"

namespace test {
	class TestFractal : public Test
	{
	public:
		TestFractal(GLFWwindow* window);
		~TestFractal();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		//Texture m_Texture;
		GLFWwindow* m_window;
		Shader m_Shaders[5];
		int m_currentShader; //Should be value 1 though 4, for indexes of m_Shaders. 
							 //Index 0 is the crosshair shader, and is always drawn. 
		Renderer m_Renderer;
		float m_positions[4 * 5 * 5];
		unsigned int m_indices[6 * 4 * 4];
		VertexArray m_va;
		VertexBuffer m_vb;
		VertexBufferLayout m_layout;
		IndexBuffer m_ib;
		FrameBuffer m_fb;
		//glm::vec3 m_translationA;
		glm::vec3 m_scale;
		glm::mat4 m_proj;
		glm::vec2 m_crosshair;
		glm::vec2 m_crosshairMandel;
		glm::vec2 m_offset;
		glm::vec2 m_offsetMandel;
		float m_zoom;
		float m_zoomMandel;
		float m_Exp;
		bool m_renderJulia;
		bool m_renderToTexture;
		long long m_renderTime;
		int m_scaleFactor; //For scaling window coordinates and dimensions to smaller textures
		int m_maxIter;
		int m_maxIterMax;
		int m_windowWidth;
		int m_windowHeight;
		bool m_stop; //Stop rendering if a key is pressed
		
		void generateBuffers(float* buffer, unsigned int* indexes, int rows, int cols, float screenWidth, float screenHeight);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		//glm::mat4 m_view;
	};
}
