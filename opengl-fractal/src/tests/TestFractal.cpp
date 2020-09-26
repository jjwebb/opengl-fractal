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
        0.0f, 0.0f, 0.0f, 0.0f,
        1440.0f, 0.0f, 1.0f, 0.0f,
        1440.0f, 900.0f, 1.0f, 1.0f,
        0.0f, 900.0f, 0.0f, 1.0f
    },
    m_indices{
        0, 1, 2,
        0, 2, 3
    },
    m_va(),
    m_vb(m_positions, 4 * 4 * sizeof(float)),
    m_layout(),
    m_ib(m_indices, 6),
    m_fb(window),
    //m_translationA(200, 200, 0),
    m_scale(1.0f, 1.0f, 1.0f),
    m_proj(glm::ortho(0.0f, 1440.0f, 0.0f, 900.0f, -1.0f, 1.0f)),
    m_crosshair(720.5f, 450.5f),
    m_crosshairMandel(0.0f, 0.0f),
    //m_offset(0.598333f, -0.001667f),
    m_offset(0.0f, 0.0f),
    m_offsetMandel(0.0f, 0.0f),
    m_zoom(1.0f),
    m_zoomMandel(0.0f),
    m_Exp(2.0f),
    m_renderJulia(false),
    m_renderTime(0)
    //m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
    m_layout.Push<float>(2);
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
    glfwSetKeyCallback(m_window, key_callback);

    m_Shader.SetUniformMat4f("u_MVP", m_proj);
    m_Shader.SetUniform2f("u_offset", m_offset.x, m_offset.y);
    m_Shader.SetUniform1f("u_zoom", m_zoom);
    m_Shader.SetUniform1f("u_Exp", m_Exp);
    m_Shader.SetUniform1i("u_Texture", 0);
    m_Shader.SetUniform1i("u_julia", m_renderJulia);

    OnUpdate(0.0f);
    m_Shader.SetUniform1i("u_renderToTexture", 1); //Render a single frame of the fractal to a texture that will be sampled instead of
    m_fb.renderToTexture();                        //rendering the same image over and over
    OnRender();
    m_Shader.SetUniform1i("u_renderToTexture", 0);
    m_fb.renderToScreen();
    OnRender();
}

test::TestFractal::~TestFractal()
{
    glfwSetWindowUserPointer(m_window, NULL);
    glfwSetMouseButtonCallback(m_window, NULL);
    glfwSetKeyCallback(m_window, NULL);
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
    //m_crosshair.x = (float)xp + 0.5f; 
    //m_crosshair.y = y - ((float)yp + 0.5f);
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
    
    float cx = (((((m_crosshair.x) / x)) * 4.8f - 3.0f) + 0.598333f) 
        *  m_zoom + m_offset.x; //translate screen coords to -3 to 1.8, 4.8
    float cy = (((((m_crosshair.y) / y)) * 3.0f - 1.5f) + 0.001667f) 
        * m_zoom + m_offset.y;

    //float mz = m_zoom / 2;
    //float x1 = (m_crosshair.x + m_offset.x) / (mz * mz) + (3.0f * x * mz * mz - 3.0f * x) / (4.8f * mz * mz) - m_crosshair.x;
    //float y1 = (m_crosshair.y + m_offset.y) / (mz * mz) + (1.5f * y * mz * mz - 1.5f * y) / (3.0f * mz * mz) - m_crosshair.y;

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
    ImGui::Text("Crosshair: X: %f Y: %f", m_crosshair.x, m_crosshair.y);
    //ImGui::Text("X1: %f Y1: %f", x1, y1);
    //ImGui::Text("Zoom: %lf / %lf", m_zoom, 1/m_zoom);
    ImGui::Text("Zoom: %f / %f", m_zoom, 1 / m_zoom);
    ImGui::Text("Render time: %lld", m_renderTime);
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
    if (action == GLFW_PRESS && mods != GLFW_MOD_SHIFT)
    {
        void* data = glfwGetWindowUserPointer(window);
        TestFractal* obj = static_cast<TestFractal*>(data);

        int x, y;
        glfwGetFramebufferSize(window, &x, &y);

        float cx = (((((obj->m_crosshair.x) / x)) * 4.8f - 3.0f) + 0.598333f) 
            * obj->m_zoom + obj->m_offset.x; //translate screen coords to -3 to 1.8, 4.8
        float cy = (((((obj->m_crosshair.y) / y)) * 3.0f - 1.5f) + 0.001667f) 
            * obj->m_zoom + obj->m_offset.y;

        if (!obj->m_renderJulia || mods == 0)
        {
            obj->m_offset.x = cx;
            obj->m_offset.y = cy;
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            obj->m_zoom /= 2.0f;
            obj->m_crosshair.x = x / 2 + 0.5f;
            obj->m_crosshair.y = y / 2 + 0.5f;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            obj->m_zoom *= 2.0f;
            obj->m_crosshair.x = x / 2 + 0.5f;
            obj->m_crosshair.y = y / 2 + 0.5f;
        }

        obj->m_Shader.SetUniform2f("u_offset", obj->m_offset.x, obj->m_offset.y);
        obj->m_Shader.SetUniform1f("u_zoom", obj->m_zoom);

        auto start = std::chrono::high_resolution_clock::now();
        obj->m_Shader.SetUniform1i("u_renderToTexture", 1); //Render a single frame of the fractal to a texture that will be sampled instead of
        obj->m_fb.renderToTexture();                        //rendering the same image over and over
        obj->OnRender();
        GLCall(glFinish());
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        obj->m_renderTime = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        obj->m_Shader.SetUniform1i("u_renderToTexture", 0);
        obj->m_fb.renderToScreen();
    }
}

void test::TestFractal::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        void* data = glfwGetWindowUserPointer(window);
        TestFractal* obj = static_cast<TestFractal*>(data);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        switch(key)
        { 
        //WASD move the crosshair AND the value of C (changing the julia set displayed when in julia mode),
        //effectively moving the crosshair as if the Mandelbrot set were still displayed.
        //UP DOWN LEFT RIGHT move ONLY the crosshair (leaving the julia set unchanged)
        case GLFW_KEY_W:
        case GLFW_KEY_UP:

            if (key == GLFW_KEY_W && obj->m_renderJulia)
            {
                //The crosshair could have moved with the arrow keys, we need to reset it if the last move wasn't with WASD
                if (obj->m_crosshair.y == obj->m_crosshairMandel.y && obj->m_crosshair.x == obj->m_crosshairMandel.x)
                    obj->m_crosshair.y += 20;
                else
                {
                    obj->m_crosshair.y = obj->m_crosshairMandel.y +20;
                    obj->m_crosshair.x = obj->m_crosshairMandel.x;
                }
                float cx = (((((obj->m_crosshair.x) / width)) * 4.8f - 3.0f) + 0.598333f) 
                    * obj->m_zoomMandel + obj->m_offsetMandel.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = (((((obj->m_crosshair.y) / height)) * 3.0f - 1.5f) + 0.001667f) 
                    * obj->m_zoomMandel + obj->m_offsetMandel.y;
                obj->m_Shader.SetUniform2f("u_cVals", cx, cy);
                mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 2);
                //obj->m_offsetMandel.x = obj->m_offset.x;
                //obj->m_offsetMandel.y = obj->m_offset.y;
                obj->m_crosshairMandel.x = obj->m_crosshair.x;
                obj->m_crosshairMandel.y = obj->m_crosshair.y;
            }
            else
            {
                obj->m_crosshair.y += 20;
                if (obj->m_crosshair.y > height)
                {
                    obj->m_crosshair.y += height / 4;
                    float tmp = obj->m_crosshair.x;
                    obj->m_crosshair.x = width / 2 + 0.5f;
                    mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 0);
                    //obj->m_crosshair.y = 2.5f;
                    obj->m_crosshair.y = height / 4 + 0.5f;
                    obj->m_crosshair.x = tmp;
                }
            }
            break;
        case GLFW_KEY_S:
        case GLFW_KEY_DOWN:
            if (key == GLFW_KEY_S && obj->m_renderJulia)
            {
                if (obj->m_crosshair.y == obj->m_crosshairMandel.y && obj->m_crosshair.x == obj->m_crosshairMandel.x)
                    obj->m_crosshair.y -= 20;
                else
                {
                    obj->m_crosshair.y = obj->m_crosshairMandel.y - 20;
                    obj->m_crosshair.x = obj->m_crosshairMandel.x;
                }
                float cx = (((((obj->m_crosshair.x) / width)) * 4.8f - 3.0f) + 0.598333f) 
                    * obj->m_zoomMandel + obj->m_offsetMandel.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = (((((obj->m_crosshair.y) / height)) * 3.0f - 1.5f) + 0.001667f) 
                    * obj->m_zoomMandel + obj->m_offsetMandel.y;
                obj->m_Shader.SetUniform2f("u_cVals", cx, cy);
                mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 2);
                //obj->m_offsetMandel.x = obj->m_offset.x;
                //obj->m_offsetMandel.y = obj->m_offset.y;
                obj->m_crosshairMandel.x = obj->m_crosshair.x;
                obj->m_crosshairMandel.y = obj->m_crosshair.y;
            }
            else
            {
                obj->m_crosshair.y -= 20;
                if (obj->m_crosshair.y < 0)
                {
                    obj->m_crosshair.y -= height / 4;
                    float tmp = obj->m_crosshair.x;
                    obj->m_crosshair.x = width / 2 + 0.5f;
                    mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 0);
                    //obj->m_crosshair.y = height - 2.5f;
                    obj->m_crosshair.y = height - (height / 4) - 0.5f;
                    obj->m_crosshair.x = tmp;
                }
            }
            break;
        case GLFW_KEY_A:
        case GLFW_KEY_LEFT:
            if (key == GLFW_KEY_A && obj->m_renderJulia)
            {
                if (obj->m_crosshair.y == obj->m_crosshairMandel.y && obj->m_crosshair.x == obj->m_crosshairMandel.x)
                    obj->m_crosshair.x -= 20;
                else
                {
                    obj->m_crosshair.y = obj->m_crosshairMandel.y;
                    obj->m_crosshair.x = obj->m_crosshairMandel.x - 20;
                }
                float cx = (((((obj->m_crosshair.x) / width)) * 4.8f - 3.0f) + 0.598333f) 
                    * obj->m_zoomMandel + obj->m_offsetMandel.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = (((((obj->m_crosshair.y) / height)) * 3.0f - 1.5f) + 0.001667f) 
                    * obj->m_zoomMandel + obj->m_offsetMandel.y;
                obj->m_Shader.SetUniform2f("u_cVals", cx, cy);
                mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 2);
                //obj->m_offsetMandel.x = obj->m_offset.x;
                //obj->m_offsetMandel.y = obj->m_offset.y;
                obj->m_crosshairMandel.x = obj->m_crosshair.x;
                obj->m_crosshairMandel.y = obj->m_crosshair.y;
            }
            else
            {
                obj->m_crosshair.x -= 20;
                if (obj->m_crosshair.x < 0)
                {
                    obj->m_crosshair.x -= width / 4;
                    float tmp = obj->m_crosshair.y;
                    obj->m_crosshair.y = height / 2 + 0.5f;
                    mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 0);
                    obj->m_crosshair.x = width - (width / 4) - 0.5f;
                    obj->m_crosshair.y = tmp;
                }
            }
            break;
        case GLFW_KEY_D:
        case GLFW_KEY_RIGHT:
            if (key == GLFW_KEY_D && obj->m_renderJulia)
            {
                if (obj->m_crosshair.y == obj->m_crosshairMandel.y && obj->m_crosshair.x == obj->m_crosshairMandel.x)
                    obj->m_crosshair.x += 20;
                else
                {
                    obj->m_crosshair.y = obj->m_crosshairMandel.y;
                    obj->m_crosshair.x = obj->m_crosshairMandel.x + 20;
                }
                float cx = (((((obj->m_crosshair.x) / width)) * 4.8f - 3.0f) + 0.598333f) 
                    * obj->m_zoomMandel + obj->m_offsetMandel.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = (((((obj->m_crosshair.y) / height)) * 3.0f - 1.5f) + 0.001667f) 
                    * obj->m_zoomMandel + obj->m_offsetMandel.y;
                obj->m_Shader.SetUniform2f("u_cVals", cx, cy);

                mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 2);
                //obj->m_offsetMandel.x = obj->m_offset.x;
                //obj->m_offsetMandel.y = obj->m_offset.y;
                obj->m_crosshairMandel.x = obj->m_crosshair.x;
                obj->m_crosshairMandel.y = obj->m_crosshair.y;
            }
            else
            {
                obj->m_crosshair.x += 20;
                if (obj->m_crosshair.x > width)
                {
                    obj->m_crosshair.x += width / 4;
                    float tmp = obj->m_crosshair.y;
                    obj->m_crosshair.y = height / 2 + 0.5f;
                    mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 0);
                    obj->m_crosshair.x = width / 4 + 0.5f;
                    obj->m_crosshair.y = tmp;
                }
            }
            break;

        //BACKSPACE zooms out, ENTER zooms in
        case GLFW_KEY_BACKSPACE:
            mouse_button_callback(window, GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS, 0);
            break;
        case GLFW_KEY_ENTER:
            mouse_button_callback(window, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0);
            break;

        //Pressing the J key activates Julia mode and renders a julia set for the given
        //point C on the mandelbrot set pointed to by the crosshair
        case GLFW_KEY_J:
        {
            if (!obj->m_renderJulia)
            {
                float cx = (((((obj->m_crosshair.x) / width)) * 4.8f - 3.0f) + 0.598333f) 
                    * obj->m_zoom + obj->m_offset.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = (((((obj->m_crosshair.y) / height)) * 3.0f - 1.5f) + 0.001667f) 
                    * obj->m_zoom + obj->m_offset.y;
                obj->m_Shader.SetUniform2f("u_cVals", cx, cy);
                
                obj->m_offsetMandel.x = obj->m_offset.x;
                obj->m_offsetMandel.y = obj->m_offset.y;
                obj->m_crosshairMandel.x = obj->m_crosshair.x;
                obj->m_crosshairMandel.y = obj->m_crosshair.y;
                obj->m_offset.x = cx;
                obj->m_offset.y = cy;
                obj->m_zoomMandel = obj->m_zoom;

            }
            else
            {
                obj->m_offset.x = obj->m_offsetMandel.x;
                obj->m_offset.y = obj->m_offsetMandel.y;
                obj->m_crosshair.x = obj->m_crosshairMandel.x;
                obj->m_crosshair.y = obj->m_crosshairMandel.y;
                obj->m_zoom = obj->m_zoomMandel;
            }
            obj->m_Shader.SetUniform1i("u_julia", !obj->m_renderJulia);
            obj->m_renderJulia = !obj->m_renderJulia;
            mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 0);
            obj->m_crosshair.x = width / 2 + 0.5f;
            obj->m_crosshair.y = height / 2 + 0.5f;
            break;
        }

        //Pressing N raises the exponent in Z^n + C by 1, P lowers it by 1
        case GLFW_KEY_N:
            obj->m_Shader.SetUniform1f("u_Exp", obj->m_Exp + 1.0f);
            obj->m_Exp++;
            mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 0);
            obj->m_crosshair.x = width / 2 + 0.5f;
            obj->m_crosshair.y = height / 2 + 0.5f;
            break;
        case GLFW_KEY_P:
            if (obj->m_Exp > 2.0f)
            {
                obj->m_Exp--;
                obj->m_Shader.SetUniform1f("u_Exp", obj->m_Exp);
                //obj->m_Exp--;
                mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 0);
                obj->m_crosshair.x = width / 2 + 0.5f;
                obj->m_crosshair.y = height / 2 + 0.5f;
            }
            break;
        case GLFW_KEY_ESCAPE:
          glfwSetWindowShouldClose(window, GLFW_TRUE);
          break;
        }
    }
}
