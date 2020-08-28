#pragma once

#include "Test.h"
#include "Texture.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

namespace test {
	class TestTexture : public Test
	{
	public:
		TestTexture();
		~TestTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		Texture m_Texture;
		Shader m_Shader;
		Renderer m_Renderer;
		float m_positions[16];
		unsigned int m_indices[6];
		VertexArray m_va;
		VertexBuffer m_vb;
		VertexBufferLayout m_layout;
		IndexBuffer m_ib;
		glm::vec3 m_translationA;
		glm::vec3 m_translationB;
		glm::vec3 m_scale;
		glm::mat4 m_proj;
		glm::mat4 m_view;
	};
}