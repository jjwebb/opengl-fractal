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

    //GLFW must be initialized before GLEW
    if (!glfwInit())
        return -1;

    //Raspberry Pi 4B supports up to OpenGL 3.1 ES
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    //Create a fullscreen window
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window = glfwCreateWindow(mode->width, mode->height, "Fractal Explorer", glfwGetPrimaryMonitor(), NULL);
    const char * err;
    int code = glfwGetError(&err);
    if (err != GLFW_NO_ERROR || !window)
    {
        std::cout<<"GLFW error "<<code<<": "<<err<<std::endl;
        glfwTerminate();
        return -1;
    }

    //Lock aspect ratio to 16:9
    glfwSetWindowAspectRatio(window, 16, 9);

    //Make the window's context current
    glfwMakeContextCurrent(window);

    //Set refresh rate (0 = unlimited)
    glfwSwapInterval(0);

    //Initalize GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "glewInit error!";

    //Output OpenGL version to the console
    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        //Set up ImGUI
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);

        //Output GLSL version to the console
        char* str = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        std::cout<<"Version: "<<str<<std::endl;

        //Tell ImGui which GLSL version to use
        ImGui_ImplOpenGL3_Init("#version 300 es");

        //Load custom font (Courier New Bold)
        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts->AddFontFromFileTTF("res/fonts/courbd.ttf", 18.0f);
        IM_ASSERT(font != 0);

        Fractal fractal(window);

        //Main loop -- runs until Esc is pressed
        while (!glfwWindowShouldClose(window))
        {
            //Loop will render a frame at a low resolution and keep rendering until
            //the final pass is made at full res
            while (!fractal.doneRendering())
            {
                //Coordinate info/GUI is toggled with i key
                if (fractal.showGUI())
                {
                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui_ImplGlfw_NewFrame();
                    ImGui::NewFrame();
                    
                    fractal.MainRenderLoop();

                    ImGui::Begin("Mandelbrot", 0, ImGuiWindowFlags_NoTitleBar
                        | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);

                    fractal.OnImGuiRender();

                    ImGui::End();
                    ImGui::Render();
                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                }
                else
                    fractal.MainRenderLoop();

                //Show the rendered frame on the screen
                glfwSwapBuffers(window);
            }
            //Wait for a keypress
            glfwWaitEvents();
        }
    }

    //Clean everything up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
