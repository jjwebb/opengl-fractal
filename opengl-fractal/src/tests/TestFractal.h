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
		Shader m_Shader;
		Renderer m_Renderer;
		float m_positions[16];
		unsigned int m_indices[6];
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
		long long m_renderTime;
		//void fractalWindow();
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		//glm::mat4 m_view;
	};
}
