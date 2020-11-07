#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Fractal.h"

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

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);

        char* str = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        std::cout<<"Version: "<<str<<std::endl;

        ImGui_ImplOpenGL3_Init("#version 300 es");
        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts->AddFontFromFileTTF("res/fonts/courbd.ttf", 18.0f);
        IM_ASSERT(font != 0);

        Fractal fractal(window);

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

                    fractal.OnUpdate();
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
                    fractal.OnUpdate();
                    fractal.OnRender();
                }
                glfwSwapBuffers(window);
            }
            /* Poll for and process events */
            glfwWaitEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
