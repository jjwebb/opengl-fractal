#include "TestFractal.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

test::TestFractal::TestFractal(GLFWwindow* window)
    : //m_Texture("res/textures/peach.png"),
    //m_window(glfwCreateWindow(1440, 900, "Mandelbrot", NULL, NULL)),
    m_window(window),
    m_Shader("res/shaders/Fractal.shader"),
    m_Renderer(),
    m_positions{
        0.0f, 0.0f, 
        1440.0f, 0.0f, 
        1440.0f, 900.0f, 
        0.0f, 900.0f, 
    },
    m_indices{
        0, 1, 2,
        0, 2, 3
    },
    m_va(),
    m_vb(m_positions, 2 * 4 * sizeof(float)),
    m_layout(),
    m_ib(m_indices, 6),
    //m_translationA(200, 200, 0),
    m_scale(1.0f, 1.0f, 1.0f),
    m_proj(glm::ortho(0.0f, 1440.0f, 0.0f, 900.0f, -1.0f, 1.0f)),
    m_crosshair(720.5f, 450.5f),
    //m_offset(0.598333f, -0.001667f),
    m_offset(0.0f, 0.0f),
    m_zoom(1.0f)
    //m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
    m_layout.Push<float>(2);
    //m_layout.Push<float>(2);

    m_va.AddBuffer(m_vb, m_layout);

    m_Shader.Bind();
    //m_Shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
    //m_Texture.Bind();
    //m_Shader.SetUniform1i("u_Texture", 0);
    //m_Shader.SetUniform1i("u_Texture", 0);
    
    //glfwSetWindowAspectRatio(m_window, 16, 10);
    //if (!m_window)
    //{
    //    glfwTerminate();
    //    return;
    //}


    ///* Make the window's context current */
    //glfwMakeContextCurrent(m_window);

    //glfwSwapInterval(1);

    //glfwSetCursorPosCallback(m_window, cursorCallback);

    glfwSetWindowUserPointer(m_window, this);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);

    m_Shader.SetUniformMat4f("u_MVP", m_proj);
    m_Shader.SetUniform2f("u_offset", m_offset.x, m_offset.y);
    m_Shader.SetUniform1f("u_zoom", m_zoom);
}

test::TestFractal::~TestFractal()
{
    glfwSetWindowUserPointer(m_window, NULL);
    glfwSetMouseButtonCallback(m_window, NULL);
}

void test::TestFractal::OnUpdate(float deltaTime)
{
    int x, y;

    glfwGetFramebufferSize(m_window, &x, &y);
    //float xf = (float)x;
    //float yf = (float)y;
    m_Shader.SetUniform2f("u_FramebufferSize", (float)x, (float)y);
    //yf = xf * 0.625f;
    //m_proj = glm::ortho(0.0f, xf, 0.0f, yf, -1.0f, 1.0f);
    //m_proj = glm::ortho(xf, 0.0f, yf, 0.0f, -1.0f, 1.0f);
    //m_scale[0] = xf / 1440;
    //m_scale[1] = yf / 900;
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLCall(glViewport(0, 0, x, y));

    double xp, yp;
    glfwGetCursorPos(m_window, &xp, &yp);
    m_crosshair.x = (float)xp + 0.5f; 
    m_crosshair.y = y - ((float)yp + 0.5f);
    m_Shader.SetUniform2f("u_crossHairCoord", m_crosshair.x, m_crosshair.y);
}

void test::TestFractal::OnRender()
{
    m_Renderer.Clear();

    //m_Shader.Bind();
    //m_ib.Bind();
    {
       // glm::mat4 scale = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
       //     , glm::vec3(m_scale[0], m_scale[1], 1.0f));
        //m_scale[1] = m_scale[0];
        //glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), m_translationA),
        //    glm::vec3(m_scale[0], m_scale[1], m_scale[2]));//scale first, translation second
        //glm::mat4 mvp = m_proj * m_view * model;
        //glm::mat4 mvp = m_proj * scale;

        //m_Shader.SetUniformMat4f("u_MVP", m_proj);
        //fractalWindow();
        //shader.Bind();
        m_Renderer.Draw(m_va, m_ib, m_Shader);
        
    }

}

void test::TestFractal::OnImGuiRender()
{
    int x, y;
    glfwGetFramebufferSize(m_window, &x, &y);
    
    float cx = (((((m_crosshair.x) / x)) * 4.8f - 3.0f) + 0.598333f) *  m_zoom + m_offset.x; //translate screen coords to -3 to 1.8, 4.8
    float cy = (((((m_crosshair.y) / y)) * 3.0f - 1.5f) + 0.001667f) * m_zoom + m_offset.y;

    float mz = m_zoom / 2;
    float x1 = (m_crosshair.x + m_offset.x) / (mz * mz) + (3.0f * x * mz * mz - 3.0f * x) / (4.8f * mz * mz) - m_crosshair.x;
    float y1 = (m_crosshair.y + m_offset.y) / (mz * mz) + (1.5f * y * mz * mz - 1.5f * y) / (3.0f * mz * mz) - m_crosshair.y;

    double xp, yp;
    glfwGetCursorPos(m_window, &xp, &yp);
    float ox =  (-x / 2.0f + (float)xp);
    float oy =  (y / 2.0f - (float)yp);

    //ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //ImGui::SliderFloat("Zoom: ", &m_zoom, -4.0f, 4.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //ImGui::SliderFloat3("Scale: ", &m_scale[0], 0.0f, 10.0f);
    ImGui::Text("C real: %f C imag: %f", cx, cy);
    ImGui::Text("X offset: %f Y offset: %f", m_offset.x, m_offset.y);
    //ImGui::SliderFloat2("Offset: ", &m_offset.x, -2000.0f, 2000.0f);
    ImGui::Text("X offset add: %f Y offset add: %f", ox, oy);
    //ImGui::Text("X1: %f Y1: %f", x1, y1);
    ImGui::Text("Zoom: %lf / %lf", m_zoom, 1/m_zoom);
    ImGui::Text("Zoom: %f / %f", m_zoom, 1 / m_zoom);
    if (ImGui::Button("Reset"))
    {
        m_offset.x = 0.0f;
        m_offset.y = 0.0f;
        m_zoom = 1.0f;
        m_Shader.SetUniform2f("u_offset", m_offset.x, m_offset.y);
        m_Shader.SetUniform1f("u_zoom", m_zoom);
    }
}

//void test::TestFractal::fractalWindow()
//{
//    double xpos, ypos;
//    while (!glfwWindowShouldClose(m_window))
//    {
//        glfwGetCursorPos(m_window, &xpos, &ypos);
//        //GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
//        GLCall(glClear(GL_COLOR_BUFFER_BIT));
//        //renderer.Clear();
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        /*if (ImGui::Button("Test Color"))
//            test = &testClearColor;
//        else if (ImGui::Button("Test Textures"))
//            test = &testTexture;*/
//
//            //OnUpdate(0.0f);
//            //OnRender();
//            ImGui::Begin("Test");
//            ImGui::Text("X pos: %f Y pos: %f", xpos, ypos);
//            OnImGuiRender();
//            ImGui::End();
//        }
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(m_window);
//
//        /* Poll for and process events */
//        glfwPollEvents();
//}

void test::TestFractal::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    

}

void test::TestFractal::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{    
    void* data = glfwGetWindowUserPointer(window);
    TestFractal* obj = static_cast<TestFractal*>(data);

    int x, y;
    glfwGetFramebufferSize(window, &x, &y);

    double xp, yp;
    glfwGetCursorPos(window, &xp, &yp);

    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        //obj->m_offset.x += (-x / 2.0f + (float)xp);
        //obj->m_offset.y += ( y / 2.0f - (float)yp);

        float cx = (((((obj->m_crosshair.x) / x)) * 4.8f - 3.0f) + 0.598333f) * obj->m_zoom + obj->m_offset.x; //translate screen coords to -3 to 1.8, 4.8
        float cy = (((((obj->m_crosshair.y) / y)) * 3.0f - 1.5f) + 0.001667f) * obj->m_zoom + obj->m_offset.y;
        
        obj->m_offset.x = cx;
        obj->m_offset.y = cy;

        obj->m_zoom /= 2.0f;

        //float x1 = (obj->m_crosshair.x + obj->m_offset.x) / (obj->m_zoom * obj->m_zoom) + (3.0f * x * obj->m_zoom * obj->m_zoom - 3.0f * x) / (4.8f * obj->m_zoom * obj->m_zoom) - obj->m_crosshair.x;
        //float y1 = (obj->m_crosshair.y + obj->m_offset.y) / (obj->m_zoom * obj->m_zoom) + (1.5f * y * obj->m_zoom * obj->m_zoom - 1.5f * y) / (3.0f * obj->m_zoom * obj->m_zoom) - obj->m_crosshair.y;
        //obj->m_offset.x += x1;
        //obj->m_offset.y += y1;
        //obj->m_offset.x = (-4.8f * obj->m_crosshair.x + 3.0f * (float)xp) / (4.8f * obj->m_offset.x);

        //obj->m_offset.x *= obj->m_zoom;
        //obj->m_offset.y *= obj->m_zoom;
        obj->m_Shader.SetUniform2f("u_offset", obj->m_offset.x, obj->m_offset.y);
        obj->m_Shader.SetUniform1f("u_zoom", obj->m_zoom);

    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        //if(obj->m)
        //obj->m_offset.x += (-x / 2.0f + (float)xp) / obj->m_zoom;
        //obj->m_offset.y += ( y / 2.0f - (float)yp) / obj->m_zoom;

        float cx = (((((obj->m_crosshair.x) / x)) * 4.8f - 3.0f) + 0.598333f) * obj->m_zoom + obj->m_offset.x; //translate screen coords to -3 to 1.8, 4.8
        float cy = (((((obj->m_crosshair.y) / y)) * 3.0f - 1.5f) + 0.001667f) * obj->m_zoom + obj->m_offset.y;

        obj->m_offset.x = cx;
        obj->m_offset.y = cy;

        obj->m_zoom *= 2.0f;
        //float x1 = (obj->m_crosshair.x + obj->m_offset.x) / (obj->m_zoom * obj->m_zoom) + (3.0f * x * obj->m_zoom * obj->m_zoom - 3.0f * x) / (4.8f * obj->m_zoom * obj->m_zoom) - obj->m_crosshair.x;
        //float y1 = (obj->m_crosshair.y + obj->m_offset.y) / (obj->m_zoom * obj->m_zoom) + (1.5f * y * obj->m_zoom * obj->m_zoom - 1.5f * y) / (3.0f * obj->m_zoom * obj->m_zoom) - obj->m_crosshair.y;
        //obj->m_offset.x += x1;
        //obj->m_offset.y += y1;
        //obj->m_offset.x = (-4.8f * obj->m_crosshair.x + 3.0f * (float)xp) / (4.8f * obj->m_offset.x);

        //obj->m_offset.x /= obj->m_zoom;
        //obj->m_offset.y /= obj->m_zoom;
        obj->m_Shader.SetUniform2f("u_offset", obj->m_offset.x, obj->m_offset.y);
        obj->m_Shader.SetUniform1f("u_zoom", obj->m_zoom);
    }

}

