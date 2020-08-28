#include "TestTexture.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

test::TestTexture::TestTexture()
    : m_Texture("res/textures/peach.png"),
    m_Shader("res/shaders/Basic.shader"),
    m_Renderer(),
    m_positions{
        -50.0f, -50.0f, 0.0f, 0.0f, //0 second 2 floats for texture mapping
        50.0f, -50.0f, 1.0f, 0.0f, //1
        50.0f, 50.0f, 1.0f, 1.0f, //2
        -50.0f, 50.0f, 0.0f, 1.0f //3
    },
    m_indices{
        0, 1, 2,
        0, 2, 3
    },
    m_va(),
    m_vb(m_positions, 4 * 4 * sizeof(float)),
    m_layout(),
    m_ib(m_indices, 6),
    m_translationA(200, 200, 0),
    m_translationB(400, 200, 0),
    m_scale(1.0f, 0.0f, 0.0f),
    m_proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
    m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
    m_layout.Push<float>(2);
    m_layout.Push<float>(2);

    m_va.AddBuffer(m_vb, m_layout);

    m_Shader.Bind();
    //m_Shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    m_Texture.Bind();
    m_Shader.SetUniform1i("u_Texture", 0);
    //m_Shader.SetUniform1i("u_Texture", 0);
}

test::TestTexture::~TestTexture()
{
}

void test::TestTexture::OnUpdate(float deltaTime)
{
    
}

void test::TestTexture::OnRender()
{
    m_Renderer.Clear();

    //m_Shader.Bind();
    //m_ib.Bind();
    {
        m_scale[1] = m_scale[0];
        glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), m_translationA),
            glm::vec3(m_scale[0], m_scale[1], m_scale[2]));
        glm::mat4 mvp = m_proj * m_view * model;
        m_Shader.SetUniformMat4f("u_MVP", mvp);
        //shader.Bind();
        m_Renderer.Draw(m_va, m_ib, m_Shader);
    }

    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
        glm::mat4 mvp = m_proj * m_view * model;
        m_Shader.SetUniformMat4f("u_MVP", mvp);
        //shader.Bind();
        m_Renderer.Draw(m_va, m_ib, m_Shader);
    }
}

void test::TestTexture::OnImGuiRender()
{
    ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.0f, 960.0f);
    ImGui::SliderFloat3("Scale: ", &m_scale[0], 0.0f, 10.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
