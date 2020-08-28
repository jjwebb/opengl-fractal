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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"
#include "tests/TestFractal.h"

void cursorCallback(GLFWwindow* window, double xpos, double ypos);
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1440, 900, "Mandelbrot", NULL, NULL);
    glfwSetWindowAspectRatio(window, 16, 10);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glfwSetCursorPosCallback(window, cursorCallback);

    if (glewInit() != GLEW_OK)
        std::cout << "glewInit error!";

    std::cout << glGetString(GL_VERSION) << std::endl;
    {

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        //Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

        //test::TestClearColor testClearColor;
        //test::TestTexture testTexture;
        //test::Test* test = &testClearColor;
        //test = &testTexture;

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture>("Texture");
        testMenu->RegisterTest<test::TestFractal>("Fractal", window);

        double xpos, ypos;

        int x, y;

        glfwGetFramebufferSize(window, &x, &y);

        while (!glfwWindowShouldClose(window))
        {
            glfwGetCursorPos(window, &xpos, &ypos);
            //GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT));
            //renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            /*if (ImGui::Button("Test Color"))
                test = &testClearColor;
            else if (ImGui::Button("Test Textures"))
                test = &testTexture;*/

            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                ImGui::Text("X pos: %f Y pos: %f", xpos, y - ypos);
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        if (currentTest != testMenu)
            delete testMenu;
        delete currentTest;
        //GLCall(glDeleteProgram(shader));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    //std::cout << "X: " << xpos << " Y: " << ypos << std::endl;
}