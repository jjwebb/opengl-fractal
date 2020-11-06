#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"
#include "tests/TestFractal.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Mandelbrot", glfwGetPrimaryMonitor(), NULL);
    const char * err;
    int code = glfwGetError(&err);
    if (err != GLFW_NO_ERROR)
    {
        std::cout<<"GLFW error "<<code<<": "<<err<<std::endl;
    }
    glfwSetWindowAspectRatio(window, 16, 9);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK)
        std::cout << "glewInit error!";

    std::cout << glGetString(GL_VERSION) << std::endl;
    {

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        //Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        int versions = 0;
        glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &versions);
        char* str = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        std::cout<<"Version: "<<str<<std::endl;
        ImGui_ImplOpenGL3_Init("#version 300 es");
        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts->AddFontFromFileTTF("res/fonts/courbd.ttf", 18.0f);
        IM_ASSERT(font != 0);

        //test::TestClearColor testClearColor;
        //test::TestTexture testTexture;
        //test::Test* test = &testClearColor;
        //test = &testTexture;

        /*test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture>("Texture");
        testMenu->RegisterTest<test::TestFractal>("Fractal", window);*/

        test::TestFractal fractal(window);

        double xpos, ypos;

        int x, y;

        glfwGetFramebufferSize(window, &x, &y);

        while (!glfwWindowShouldClose(window))
        {
            //glfwGetCursorPos(window, &xpos, &ypos);
            //GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            //GLCall(glClear(GL_COLOR_BUFFER_BIT));
            //renderer.Clear();
            while (!fractal.doneRendering())
            {
                if (fractal.showGUI())
                {
                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui_ImplGlfw_NewFrame();
                    ImGui::NewFrame();

                    fractal.OnUpdate(0.0f);
                    fractal.OnRender();

                    ImGui::Begin("Mandelbrot", 0, ImGuiWindowFlags_NoTitleBar
                        | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);

                    fractal.OnImGuiRender();

                    ImGui::End();
                    ImGui::Render();
                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                }
                else
                {
                    fractal.OnUpdate(0.0f);
                    fractal.OnRender();
                }
                glfwSwapBuffers(window);
            }
            /* Poll for and process events */
            glfwWaitEvents();
        }
        //GLCall(glDeleteProgram(shader));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
