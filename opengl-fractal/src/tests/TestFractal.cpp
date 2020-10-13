#include<iomanip>
#include "TestFractal.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define XMUL 5.0f
//#define XSUBT 2.8125f
//With a X/Y ratio of 5.0/2.8125 (16:9) the center of the screen is at
//C coords (-.310547, .001953). We have to add this into our equation for 
//the zoom to scale properly
#define XSUBT 2.501953f
#define YMUL 2.8125f
//#define YSUBT 1.40625f
#define YSUBT 1.408203f
//#define XADD 0.310547f
//#define YADD -0.001953f

test::TestFractal::TestFractal(GLFWwindow* window)
    : //m_Texture("res/textures/peach.png"),
    //m_window(glfwCreateWindow(1440, 900, "Mandelbrot", NULL, NULL)),
    m_window(window),
    m_Shaders{Shader("res/shaders/Crosshair.shader"),  //[0] -- Crosshair shader
              Shader("res/shaders/Mandelbrot.shader"), //[1] -- Mandelbrot only shader
              Shader("res/shaders/Multibrot.shader"),  //[2] -- Multibrot only shader
              Shader("res/shaders/Julia.shader"),      //[3] -- Julia set only shader
              Shader("res/shaders/Multijulia.shader")},//[4] -- Multi-julia only shader
    m_currentShader(1),
    m_Renderer(),
    /*m_positions{
        0.0f, 0.0f, 0.0f, 0.0f,
        1920.0f, 0.0f, 1.0f, 0.0f,
        1920.0f, 1080.0f, 1.0f, 1.0f,
        0.0f, 1080.0f, 0.0f, 1.0f
    },
    m_indices{
        0, 1, 2,
        0, 2, 3
    },*/

    m_positions{
        0.0f, 0.0f, 0.0f, 0.0f, //0
        960.0f, 0.0f, 0.5f, 0.0f,//1
        960.0f, 540.0f, 0.5f, 0.5f,//2
        0.0f, 540.0f, 0.0f, 0.5f,//3

        1920.0f, 0.0f, 1.0f, 0.0f,//4
        1920.0f, 540.0f, 1.0f, 0.5f,//5

        1920.0f, 1080.0f, 1.0f, 1.0f,//6
        960.0f, 1080.0f, 0.5f, 1.0f,//7

        0.0f, 1080.0f, 0.0f, 1.0f//8
    },
    
    m_indices{
        0, 1, 2,
        0, 2, 3,

        1, 4, 5,
        1, 5, 2,

        2, 5, 6,
        2, 6, 7,

        3, 2, 7,
        3, 7, 8
    },


    m_va(),
    m_vb(m_positions, 9 * 4 * sizeof(float)),
    //m_vb(m_positions, 4 * 4 * sizeof(float)),
    m_layout(),
    m_ib(m_indices, 24),
    //m_ib(m_indices, 6),
    m_fb(window),
    //m_translationA(200, 200, 0),
    m_scale(1.0f, 1.0f, 1.0f),
    m_proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
    m_crosshair(720.5f, 450.5f),
    m_crosshairMandel(0.0f, 0.0f),
    //m_offset(XADD, -YADD),
    m_offset(0.0f, 0.0f),
    m_offsetMandel(0.0f, 0.0f),
    m_zoom(1.0f),
    m_zoomMandel(0.0f),
    m_Exp(2.0f),
    m_renderJulia(false),
    m_renderToTexture(true),
    m_renderTime(0),
    m_scaleFactor(1),
    m_maxIter(200),
    m_maxIterMax(400),
    m_windowWidth(0),
    m_windowHeight(0),
    m_stop(false)
    //m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
    m_layout.Push<float>(2);
    m_layout.Push<float>(2);
    //m_layout.Push<float>(2);

    m_va.AddBuffer(m_vb, m_layout);

    //m_Shader.Bind();
  
    glfwSetWindowUserPointer(m_window, this);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetKeyCallback(m_window, key_callback);

    //int x, y;
    glfwGetFramebufferSize(m_window, &m_windowWidth, &m_windowHeight);
    m_crosshair.x = m_windowWidth / 2 + 0.5f;
    m_crosshair.y = m_windowHeight / 2 + 0.5f;
    //for (Shader s : m_Shaders)
    //{
    //m_Shaders[0].Bind();
    m_Shaders[0].SetUniformMat4f("u_MVP", m_proj);
        m_Shaders[0].SetUniform2f("u_FramebufferSize", (float)m_windowWidth, (float)m_windowHeight);
    //}
    
    for (int i = 1; i < 5; i++)
    {
        //m_Shaders[i].Bind();
        m_Shaders[i].SetUniformMat4f("u_MVP", m_proj);
        m_Shaders[i].SetUniform2f("u_FramebufferSize", (float)m_windowWidth, (float)m_windowHeight);
        m_Shaders[i].SetUniform2f("u_offset", m_offset.x, m_offset.y);
        m_Shaders[i].SetUniform1f("u_zoom", m_zoom);
    }
    
    //m_Shader.SetUniform1f("u_Exp", m_Exp);

    //m_Shader.SetUniform1i("u_Texture", 0);
    //m_Shader.SetUniform1i("u_julia", m_renderJulia);

    OnUpdate(0.0f);
    //m_Shader.SetUniform1i("u_renderToTexture", 1); //Render a single frame of the fractal to a texture that will be sampled instead of
    //m_fb.renderToTexture(m_scaleFactor);           //rendering the same image over and over
    //OnRender();
    //m_Shader.SetUniform1i("u_renderToTexture", 0);
    //m_fb.renderToScreen();
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

    if (m_maxIter <= m_maxIterMax)
    {
        m_Shaders[m_currentShader].SetUniform1i("ITER_MAX", m_maxIter);
        m_maxIter *= 2;
        if (m_scaleFactor == 1)
            m_scaleFactor = 2;
    }
    if (m_scaleFactor > 0)
    {
        m_Shaders[m_currentShader].SetUniform2f("u_FramebufferSize", (float)(x/m_scaleFactor), (float)(y/m_scaleFactor));
        //m_Shader.SetUniform2f("u_crossHairCoord", m_crosshair.x/m_scaleFactor, m_crosshair.y/m_scaleFactor);

        auto start = std::chrono::high_resolution_clock::now();
        m_renderToTexture = 1;
        //m_Shader.SetUniform1i("u_renderToTexture", 1); //Render a single frame of the fractal to a texture that will be sampled instead of
        m_fb.renderToTexture(m_scaleFactor);      //rendering the same image over and over
        OnRender();
        GLCall(glFinish());
        m_scaleFactor--;
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        m_renderTime = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        //m_Shader.SetUniform1i("u_renderToTexture", 0);
        m_renderToTexture = 0;
        m_fb.renderToScreen();
    }
    //m_Shaders[0].SetUniform2f("u_FramebufferSize", (float)x, (float)y);
    m_Shaders[0].SetUniform2f("u_crossHairCoord", m_crosshair.x, m_crosshair.y);
}

void test::TestFractal::OnRender()
{
    m_Renderer.Clear();
    m_stop = false;
    if(m_renderToTexture)
        m_Renderer.Draw(m_va, m_ib, m_Shaders[m_currentShader], m_stop);
    else
        m_Renderer.Draw(m_va, m_ib, m_Shaders[0]);
}

void test::TestFractal::OnImGuiRender()
{
    int x, y;
    glfwGetFramebufferSize(m_window, &x, &y);
    
    float cx = ((m_crosshair.x / x) * XMUL - XSUBT)
        * m_zoom + m_offset.x; //translate screen coords to -3 to 1.8, 4.8
    float cy = ((m_crosshair.y / y) * YMUL - YSUBT)
        * m_zoom + m_offset.y;

    double xp, yp;
    glfwGetCursorPos(m_window, &xp, &yp);
    float ox =  (-x / 2.0f + (float)xp);
    float oy =  (y / 2.0f - (float)yp);

    //ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //ImGui::SliderFloat("Zoom: ", &m_zoom, -4.0f, 4.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::SliderInt("Maxmimum iterations: ", &m_maxIterMax, 100, 3200);
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
        m_Shaders[m_currentShader].SetUniform2f("u_offset", m_offset.x, m_offset.y);
        m_Shaders[m_currentShader].SetUniform1f("u_zoom", m_zoom);
    }
}

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

        if (mods == 0)
        {
            obj->m_offset.x = ((obj->m_crosshair.x / x) * XMUL - XSUBT)
                * obj->m_zoom + obj->m_offset.x; //translate screen coords to -3 to 1.8, 4.8
            obj->m_offset.y = ((obj->m_crosshair.y / y) * YMUL - YSUBT)
                * obj->m_zoom + obj->m_offset.y;
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            obj->m_zoom /= 2.0f;
            obj->m_crosshair.x = x / 2 + 0.5f;
            obj->m_crosshair.y = y / 2 + 0.5f;
            obj->m_crosshairMandel.x = obj->m_crosshair.x;
            obj->m_crosshairMandel.y = obj->m_crosshair.y;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            obj->m_zoom *= 2.0f;
            obj->m_crosshair.x = x / 2 + 0.5f;
            obj->m_crosshair.y = y / 2 + 0.5f;
            obj->m_crosshairMandel.x = obj->m_crosshair.x;
            obj->m_crosshairMandel.y = obj->m_crosshair.y;
        }

        obj->m_Shaders[obj->m_currentShader].SetUniform2f("u_offset", obj->m_offset.x, obj->m_offset.y);
        obj->m_Shaders[obj->m_currentShader].SetUniform1f("u_zoom", obj->m_zoom);

        //auto start = std::chrono::high_resolution_clock::now();
        //This is where we set how many render passes we will do
        obj->m_scaleFactor = 3;
        obj->m_maxIter = 200;
        /*obj->m_Shader.SetUniform1i("u_renderToTexture", 1); //Render a single frame of the fractal to a texture that will be sampled instead of
        obj->m_fb.renderToTexture(obj->m_scaleFactor);      //rendering the same image over and over
        obj->OnUpdate(0);
        obj->OnRender();
        GLCall(glFinish());
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        obj->m_renderTime = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        obj->m_Shader.SetUniform1i("u_renderToTexture", 0);
        obj->m_fb.renderToScreen();
        obj->m_scaleFactor = 1;*/
    }
}

void test::TestFractal::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        void* data = glfwGetWindowUserPointer(window);
        TestFractal* obj = static_cast<TestFractal*>(data);

        obj->m_stop = true;

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
                float cx = ((obj->m_crosshair.x / width) * XMUL - XSUBT)
                    * obj->m_zoomMandel + obj->m_offsetMandel.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = ((obj->m_crosshair.y / height) * YMUL - YSUBT)
                    * obj->m_zoomMandel + obj->m_offsetMandel.y;
                obj->m_Shaders[obj->m_currentShader].SetUniform2f("u_cVals", cx, cy);
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
                float cx = ((obj->m_crosshair.x / width) * XMUL - XSUBT)
                    * obj->m_zoomMandel + obj->m_offsetMandel.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = ((obj->m_crosshair.y / height) * YMUL - YSUBT)
                    * obj->m_zoomMandel + obj->m_offsetMandel.y;
                obj->m_Shaders[obj->m_currentShader].SetUniform2f("u_cVals", cx, cy);
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
                float cx = ((obj->m_crosshair.x / width) * XMUL - XSUBT)
                    * obj->m_zoomMandel + obj->m_offsetMandel.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = ((obj->m_crosshair.y / height) * YMUL - YSUBT)
                    * obj->m_zoomMandel + obj->m_offsetMandel.y;
                obj->m_Shaders[obj->m_currentShader].SetUniform2f("u_cVals", cx, cy);
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
                float cx = ((obj->m_crosshair.x / width) * XMUL - XSUBT)
                    * obj->m_zoomMandel + obj->m_offsetMandel.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = ((obj->m_crosshair.y / height) * YMUL - YSUBT)
                    * obj->m_zoomMandel + obj->m_offsetMandel.y;
                obj->m_Shaders[obj->m_currentShader].SetUniform2f("u_cVals", cx, cy);

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
        case GLFW_KEY_H:
        {
            if (!obj->m_renderJulia)
            {
                //If Exponent == 2, render Julia set. Otherwise, render multijulia
                //(m_Shaders[3] is the shader for Julia sets, [4] renders multijulias)
                //obj->m_currentShader = obj->m_Exp == 2.0f ? 3 : 4;
                if (obj->m_Exp > 2.0f)
                {
                    obj->m_currentShader = 4;
                    obj->m_Shaders[obj->m_currentShader].SetUniform1f("u_Exp", obj->m_Exp);
                }
                else
                    obj->m_currentShader = 3;
                float cx = ((obj->m_crosshair.x / width) * XMUL - XSUBT)
                    * obj->m_zoom + obj->m_offset.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = ((obj->m_crosshair.y / height) * YMUL - YSUBT)
                    * obj->m_zoom + obj->m_offset.y;
                obj->m_Shaders[obj->m_currentShader].SetUniform2f("u_cVals", cx, cy);
                
                obj->m_offsetMandel.x = cx;
                obj->m_offsetMandel.y = cy;
                obj->m_crosshairMandel.x = width / 2 + 0.5f;
                obj->m_crosshairMandel.y = height / 2 + 0.5f;
                obj->m_zoomMandel = obj->m_zoom;
                if (key == GLFW_KEY_H)
                {
                    obj->m_offset.x = 0.0f;
                    obj->m_offset.y = 0.0f;
                    obj->m_zoom = 1.0f;
                }

            }
            else
            {
                //If exponent == 2, render Mandelbrot. Otherwise, render multibrot
                //obj->m_currentShader = obj->m_Exp == 2.0f ? 1 : 2;
                if (obj->m_Exp > 2.0f)
                {
                    obj->m_currentShader = 2;
                    obj->m_Shaders[obj->m_currentShader].SetUniform1f("u_Exp", obj->m_Exp);
                }
                else
                    obj->m_currentShader = 1;
                obj->m_offset.x = obj->m_offsetMandel.x;
                obj->m_offset.y = obj->m_offsetMandel.y;
                obj->m_crosshair.x = obj->m_crosshairMandel.x;
                obj->m_crosshair.y = obj->m_crosshairMandel.y;
                obj->m_zoom = obj->m_zoomMandel;
            }
            //obj->m_Shader.SetUniform1i("u_julia", !obj->m_renderJulia);
            obj->m_renderJulia = !obj->m_renderJulia;
            mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 
                key == GLFW_KEY_J ? 0 : 2);
            obj->m_crosshair.x = width / 2 + 0.5f;
            obj->m_crosshair.y = height / 2 + 0.5f;
            break;
        }

        //Pressing N raises the exponent in Z^n + C by 1, P lowers it by 1
        case GLFW_KEY_N:
            obj->m_currentShader = obj->m_renderJulia ? 4 : 2;
            obj->m_Shaders[obj->m_currentShader].SetUniform1f("u_Exp", obj->m_Exp + 1.0f);
            obj->m_Exp++;
            mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 0);
            obj->m_crosshair.x = width / 2 + 0.5f;
            obj->m_crosshair.y = height / 2 + 0.5f;
            break;
        case GLFW_KEY_P:
            if (obj->m_Exp > 2.0f)
            {
                obj->m_Exp--;
                if(static_cast<int>(obj->m_Exp) == 2)
                    obj->m_currentShader = obj->m_renderJulia ? 3 : 1;
                else
                {
                    obj->m_currentShader = obj->m_renderJulia ? 4 : 2;
                    obj->m_Shaders[obj->m_currentShader].SetUniform1f("u_Exp", obj->m_Exp);
                }
                //obj->m_Exp--;
                mouse_button_callback(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 0);
                obj->m_crosshair.x = width / 2 + 0.5f;
                obj->m_crosshair.y = height / 2 + 0.5f;
            }
            break;
        case GLFW_KEY_ESCAPE:
          glfwSetWindowShouldClose(window, GLFW_TRUE);
          break;
        case GLFW_KEY_Z:
            if (!obj->m_renderJulia)
            {
                float cx = ((obj->m_crosshair.x / width) * XMUL - XSUBT)
                    * obj->m_zoom + obj->m_offset.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = ((obj->m_crosshair.y / height) * YMUL - YSUBT)
                    * obj->m_zoom + obj->m_offset.y;
                int iter;
                float x = 0.0f; float y = 0.0f; float xtmp = 0.0f;
                float xLast = 0.0f;
                for (iter = 0; iter < 100; iter++)
                {
                    xtmp = x * x - y * y + cx;
                    y = 2.0f * x * y + cy;
                    if (xtmp * xtmp + y * y > 4.0f)
                        break;
                    xLast = x;
                    x = xtmp;
                    std::cout << std::setprecision(50) << "X: " << x << " Y: " << y << " X==Xlast: " << (x == xLast) << std::endl;
                }
                std::cout << std::setprecision(50) << "X: " << x << " Y: " << y << " iter: " << iter << std::endl << std::endl;
                break;
            }
            else
            {
                float cx = ((obj->m_crosshairMandel.x / width) * XMUL - XSUBT)
                    * obj->m_zoomMandel + obj->m_offsetMandel.x; //translate screen coords to -3 to 1.8, 4.8
                float cy = ((obj->m_crosshairMandel.y / height) * YMUL - YSUBT)
                    * obj->m_zoomMandel + obj->m_offsetMandel.y;

                float zx = ((obj->m_crosshair.x / width) * XMUL - XSUBT)
                    * obj->m_zoom + obj->m_offset.x; //translate screen coords to -3 to 1.8, 4.8
                float zy = ((obj->m_crosshair.y / height) * YMUL - YSUBT)
                    * obj->m_zoom + obj->m_offset.y;
                int iter;
                float xtmp = 0.0f;
                float lastx = 0.0f;
                int n = 1;
                int n2 = 3;
                for (iter = 0; iter < 100; iter++)
                {
                    xtmp = zx * zx - zy * zy + cx;
                    zy = 2.0f * zx * zy + cy;
                    if (xtmp * xtmp + zy * zy > 4.0f)
                        break;
                    std::cout << std::setprecision(50) << "ZX: " << zx << " ZY: " << zy << " X==Xlast: " << (xtmp == lastx) << std::endl;
                    if (xtmp == lastx)
                    {
                        //iter = ITER_MAX;//commenting out this line makes for some very interesting coloration
                        break;
                    }
                    if (n2 % n == 0)
                    {
                        lastx = xtmp;
                        n2 = 0;
                        n++;
                    }
                    n2++;
                    zx = xtmp;
                }
                std::cout << std::setprecision(50) << "ZX: " << zx << " ZY: " << zy << " iter: " << iter << 
                    " n2: " << n2 << " n: " << n << std::endl << std::endl;
                break;
            }
        }
    }
}
