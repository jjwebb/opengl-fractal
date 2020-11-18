#include<iomanip>
#include<iostream>
#include "Fractal.h"

#include "glm/gtc/matrix_transform.hpp"

Fractal::Fractal(GLFWwindow* window)
    : 
    m_window(window),
    m_Shaders{Shader("res/shaders/Crosshair.shader"),  //[0] -- Crosshair shader
              Shader("res/shaders/Mandelbrot.shader"), //[1] -- Mandelbrot only shader
              Shader("res/shaders/Multibrot.shader"),  //[2] -- Multibrot only shader
              Shader("res/shaders/Julia.shader"),      //[3] -- Julia set only shader
              Shader("res/shaders/Multijulia.shader")},//[4] -- Multi-julia only shader
    m_currentShader(1),
    m_Renderer(),
    m_va(),
    m_vb(),
    m_layout(),
    m_ib(),
    m_fb(window),
    m_proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
    m_crosshair(0.0f, 0.0f),
    m_crosshairMandel(0.0f, 0.0f),
    m_crosshairMandelStatic(0.0f, 0.0f),
    m_offset(0.0f, 0.0f),
    m_offsetMandel(0.0f, 0.0f),
    m_offsetMandelStatic(0.0f, 0.0f),
    m_guiWindowPos(1655.0f, 1020.0f),
    m_zoom(1.0f),
    m_zoomMandel(0.0f),
    m_Exp(2.0f),
    m_deltaExp(0.05f),
    m_showGui(true),
    m_showRenderTime(false),
    m_renderJulia(false),
    m_renderToTexture(true),
    m_renderTime(0),
    m_scaleFactor(1),
    m_maxIter(200),
    m_maxIterMax(400),
    m_windowWidth(0),
    m_windowHeight(0),
    m_stop(false),
    m_imgChanged(false),
    m_doneRendering(false)
{
    float positions[4 * (GRIDRC + 1) * (GRIDRC + 1)];//Temporary vertex buffer to send to the GPU
    unsigned int indices[6 * GRIDRC * GRIDRC];       //Temporary index buffer to send to the GPU

    //Produce our arrays to represent our render grid for the specified grid dims
    generateBuffers(positions, indices, GRIDRC, GRIDRC, 1920.0f, 1080.0f);

    //Send the buffers to the GPU through the VertexBuffer and IndexBuffer classes
    m_vb.init(positions, (GRIDRC + 1) * (GRIDRC + 1) * 4 * sizeof(float));
    m_ib.init(indices, 6 * GRIDRC * GRIDRC);

    //Create the layout for our vertex buffer
    m_layout.Push<float>(2);
    m_layout.Push<float>(2);

    //Send the layout to the GPU so it knows how to process our buffer
    m_va.AddBuffer(m_vb, m_layout);

    //Allow glfw to access data from this class object
    glfwSetWindowUserPointer(m_window, this);

    //Set event handler
    glfwSetKeyCallback(m_window, key_callback);

    //Get the size of the window (screen)
    glfwGetFramebufferSize(m_window, &m_windowWidth, &m_windowHeight);

    //Set crosshair to the center of the screen
    resetCrosshair();

    //Send variables to the GPU (must be done for each shader program)
    m_Shaders[0].SetUniformMat4f("u_MVP", m_proj);
    m_Shaders[0].SetUniform2f("u_FramebufferSize", (float)m_windowWidth, (float)m_windowHeight);
    
    for (int i = 1; i < 5; i++)
    {
        m_Shaders[i].SetUniformMat4f("u_MVP", m_proj);
        m_Shaders[i].SetUniform2f("u_FramebufferSize", (float)m_windowWidth, (float)m_windowHeight);
        m_Shaders[i].SetUniform2f("u_offset", m_offset.x, m_offset.y);
        m_Shaders[i].SetUniform1f("u_zoom", m_zoom);
        m_Shaders[i].SetUniform1i("ITER_MAX", m_maxIterMax);
    }
}

//Destroying the object closes the program and frees everything
Fractal::~Fractal()
{
    glfwSetWindowUserPointer(m_window, NULL);
    glfwSetKeyCallback(m_window, NULL);
}

//Main logic loop - prepare to render on the GPU
void Fractal::MainRenderLoop()
{
    m_imgChanged = false;

    //Double the maximum iterations of the fractal equation every render until the specified threshold
    if (m_maxIter <= m_maxIterMax)
    {
        m_Shaders[m_currentShader].SetUniform1i("ITER_MAX", m_maxIter);
        m_maxIter *= 2;
        if (m_scaleFactor == 1)//ONLY render at full scale resolution once we've reached the specified threshold
            m_scaleFactor = 2;
    }

    //Render at progressively higher resolutions until full-scale 
    if (m_scaleFactor > 0)
    {
        //Set new window size to the appropriately scaled-down size
        m_Shaders[m_currentShader].SetUniform2f("u_FramebufferSize", 
            (float)(m_windowWidth/m_scaleFactor), (float)(m_windowHeight/m_scaleFactor));

        auto start = std::chrono::high_resolution_clock::now();

        /*Render a single frame of the fractal to a texture that will be sampled 
        instead of rendering the same image over and over*/
        m_renderToTexture = 1;
        m_fb.renderToTexture(m_scaleFactor);
        Render();
        if (m_stop)//Render stops if a key is pressed mid-render
        { 
            if (m_imgChanged)
            {
                MainRenderLoop(); 
                return; 
            }
            else
                m_fb.renderToTexture(m_scaleFactor + 1);
        }
        else
            m_scaleFactor--;
        GLCall(glFinish());
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        m_renderTime = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        m_renderToTexture = 0;
        m_fb.renderToScreen();
    }

    m_Shaders[0].SetUniform2f("u_crossHairCoord", m_crosshair.x, m_crosshair.y);

    //Keep running the render loop until we're at max iterations and full resolution
    if (m_maxIter >= m_maxIterMax && m_scaleFactor == 0)
        m_doneRendering = true;

    Render();
}

//Send the actual render/draw call to the GPU
void Fractal::Render()
{
    m_stop = false;
    if(m_renderToTexture)
        m_Renderer.Draw(m_va, m_ib, m_Shaders[m_currentShader], m_stop, m_scaleFactor, m_maxIter, m_imgChanged);
    else
        m_Renderer.Draw(m_va, m_ib, m_Shaders[SHADER_CROSSHAIR]);
}

//Renders text information about the fractal -- toggle with 'i'
void Fractal::OnImGuiRender()
{    
    if (m_Exp != 2.0f)
    {
        ImGui::Text("Z |-> Z^d + C");
        ImGui::Text("d =  %.2f (+- %.2f)", m_Exp, m_deltaExp);
    }

    if (m_renderJulia)
    {
        float zx = crosshairCoordX();
        float zy = crosshairCoordY();

        float cx = crosshairMandelCoordX();
        float cy = crosshairMandelCoordY();

        char chC = cy >= 0.0f ? '+' : '-';
        char chZ = zy >= 0.0f ? '+' : '-';
        ImGui::Text(cx >= 0.0f ? "C =  %f %c %fi" : "C = %f %c %fi", cx, chC, std::abs(cy));
        ImGui::Text(zx >= 0.0f ? "Z =  %f %c %fi" : "Z = %f %c %fi", zx, chZ, std::abs(zy));
    }
    else
    {
        float cx = crosshairCoordX();
        float cy = crosshairCoordY();

        char chC = cy >= 0.0f ? '+' : '-';
        ImGui::Text(cx >= 0.0f ? "C =  %f %c %fi" : "C = %f %c %fi", cx, chC, std::abs(cy));
    }

    ImGui::Text("Scale: %f (%ix)", m_zoom, (int)(1 / m_zoom));
    if(m_showRenderTime)
        ImGui::Text("Render time: %lld", m_renderTime);
}

void Fractal::zoom(bool zoomIn)
{
    setOffset();
    //Dividing reduces the distance between points, which means the image zooms in
    m_zoom = zoomIn ? m_zoom / 2.0f : m_zoom * 2.0f;
    m_offsetMandel.x = crosshairMandelCoordX();
    m_offsetMandel.y = crosshairMandelCoordY();
    resetCrosshair();
    resetCrosshairMandel();
    m_imgChanged = true;
    m_scaleFactor = 2;
    m_maxIter = m_maxIterMax;
    m_Shaders[m_currentShader].SetUniform1f("u_zoom", m_zoom);
}

void Fractal::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        //GLFW allows us to set a pointer to a specified object to access member data
        void* data = glfwGetWindowUserPointer(window);
        Fractal* obj = static_cast<Fractal*>(data);

        obj->m_stop = true;
        obj->m_doneRendering = false;

        int width = obj->m_windowWidth; 
        int height = obj->m_windowHeight;

        switch(key)
        { 
        /*WASD move the crosshair AND the value of C (changing the julia set displayed when in julia mode),
          effectively moving the crosshair as if the Mandelbrot set were still displayed.
          UP DOWN LEFT RIGHT move ONLY the crosshair (leaving the julia set unchanged)*/
        case GLFW_KEY_W:
        case GLFW_KEY_UP:

            if (key == GLFW_KEY_W && obj->m_renderJulia)
            {
                obj->m_crosshair.y += 20;
                obj->m_crosshairMandel.y += 20;
                obj->changeJulia();
            }
            else
            {
                obj->m_crosshair.y += 20;
                if (obj->m_crosshair.y > height)
                {
                    obj->m_crosshair.y += height / 4;
                    float tmp = obj->m_crosshair.x;
                    obj->m_crosshair.x = width / 2 + 0.5f;
                    obj->setOffset();
                    obj->resetRenderQuality();
                    obj->m_crosshair.y = height / 4 + 0.5f;
                    obj->m_crosshair.x = tmp;
                }
            }
            break;
        case GLFW_KEY_S:
        case GLFW_KEY_DOWN:
            if (key == GLFW_KEY_S && obj->m_renderJulia)
            {
                obj->m_crosshair.y -= 20;
                obj->m_crosshairMandel.y -= 20;
                obj->changeJulia();
            }
            else
            {
                obj->m_crosshair.y -= 20;
                if (obj->m_crosshair.y < 0)
                {
                    obj->m_crosshair.y -= height / 4;
                    float tmp = obj->m_crosshair.x;
                    obj->m_crosshair.x = width / 2 + 0.5f;
                    obj->setOffset();
                    obj->resetRenderQuality();
                    obj->m_crosshair.y = height - (height / 4) - 0.5f;
                    obj->m_crosshair.x = tmp;
                }
            }
            break;
        case GLFW_KEY_A:
        case GLFW_KEY_LEFT:
            if (key == GLFW_KEY_A && obj->m_renderJulia)
            {
                obj->m_crosshair.x -= 20;
                obj->m_crosshairMandel.x -= 20;
                obj->changeJulia();
            }
            else
            {
                obj->m_crosshair.x -= 20;
                if (obj->m_crosshair.x < 0)
                {
                    obj->m_crosshair.x -= width / 4;
                    float tmp = obj->m_crosshair.y;
                    obj->m_crosshair.y = height / 2 + 0.5f;
                    obj->setOffset();
                    obj->resetRenderQuality();
                    obj->m_crosshair.x = width - (width / 4) - 0.5f;
                    obj->m_crosshair.y = tmp;
                }
            }
            break;
        case GLFW_KEY_D:
        case GLFW_KEY_RIGHT:
            if (key == GLFW_KEY_D && obj->m_renderJulia)
            {
                obj->m_crosshair.x += 20;
                obj->m_crosshairMandel.x += 20;
                obj->changeJulia();
            }
            else
            {
                obj->m_crosshair.x += 20;
                if (obj->m_crosshair.x > width)
                {
                    obj->m_crosshair.x += width / 4;
                    float tmp = obj->m_crosshair.y;
                    obj->m_crosshair.y = height / 2 + 0.5f;
                    obj->setOffset();
                    obj->resetRenderQuality();
                    obj->m_crosshair.x = width / 4 + 0.5f;
                    obj->m_crosshair.y = tmp;
                }
            }
            break;

        //BACKSPACE zooms out, ENTER zooms in
        case GLFW_KEY_BACKSPACE:
            obj->zoom(false);
            break;
        case GLFW_KEY_ENTER:
            obj->zoom(true);
            break;

        /*Pressing the J key activates Julia mode and renders a julia set for the given point C
        on the mandelbrot set pointed to by the crosshair. Pressing J again puts you back on the
        mandelbrot set at whatever point you changed to (if you changed the Julia set with WASD),
        while pressing H returns you to where you were when you originally pressed J/H*/
        case GLFW_KEY_J:
        case GLFW_KEY_H:
        {
            if (!obj->m_renderJulia)
            {
                //If Exponent == 2, render Julia set. Otherwise, render multijulia
                if (obj->m_Exp != 2.0f)
                {
                    obj->m_currentShader = SHADER_MULTIJULIA;
                    obj->m_Shaders[obj->m_currentShader].SetUniform1f("u_Exp", obj->m_Exp);
                }
                else
                    obj->m_currentShader = SHADER_JULIA;

                float cx = obj->crosshairCoordX();
                float cy = obj->crosshairCoordY();
                obj->m_Shaders[obj->m_currentShader].SetUniform2f("u_cVals", cx, cy);
                
                obj->m_offsetMandel.x = cx;
                obj->m_offsetMandel.y = cy;
                obj->m_zoomMandel = obj->m_zoom;
                obj->m_offsetMandelStatic.x = obj->m_offsetMandel.x;
                obj->m_offsetMandelStatic.y = obj->m_offsetMandel.y;
                obj->m_crosshairMandelStatic.x = obj->m_crosshairMandel.x;
                obj->m_crosshairMandelStatic.y = obj->m_crosshairMandel.y;
                if (key == GLFW_KEY_H)
                {
                    obj->resetOffset();
                    obj->m_zoom = 1.0f;
                }
                else
                    obj->setOffset();
                obj->resetCrosshairMandel();

                obj->m_guiWindowPos.y -= 22.0f;
                ImGui::SetWindowPos("Mandelbrot", obj->m_guiWindowPos);
            }
            else
            {
                //Render multibrot if exponent > 2, otherwise render regular mandelbrot
                if (obj->m_Exp != 2.0f)
                {
                    obj->m_currentShader = SHADER_MULTIBROT;
                    obj->m_Shaders[obj->m_currentShader].SetUniform1f("u_Exp", obj->m_Exp);
                }
                else
                    obj->m_currentShader = SHADER_MANDELBROT;
                if (key == GLFW_KEY_H)
                {
                    obj->m_offset.x = obj->m_offsetMandelStatic.x;
                    obj->m_offset.y = obj->m_offsetMandelStatic.y;
                    obj->m_crosshair.x = obj->m_crosshairMandelStatic.x;
                    obj->m_crosshair.y = obj->m_crosshairMandelStatic.y;
                    obj->m_zoom = obj->m_zoomMandel;
                }
                else
                {
                    obj->m_offsetMandel.x = obj->crosshairMandelCoordX();
                    obj->m_offsetMandel.y = obj->crosshairMandelCoordY();
                    obj->resetCrosshairMandel();
                    obj->m_offset.x = obj->m_offsetMandel.x;
                    obj->m_offset.y = obj->m_offsetMandel.y;
                    obj->m_crosshair.x = obj->m_crosshairMandel.x;
                    obj->m_crosshair.y = obj->m_crosshairMandel.y;
                }
                
                obj->m_Shaders[obj->m_currentShader].SetUniform2f("u_offset", obj->m_offset.x, obj->m_offset.y);

                obj->m_guiWindowPos.y += 22.0f;
                ImGui::SetWindowPos("Mandelbrot", obj->m_guiWindowPos);
            }
            obj->m_renderJulia = !obj->m_renderJulia;
  
            obj->resetCrosshair();
            obj->m_Shaders[obj->m_currentShader].SetUniform1f("u_zoom", obj->m_zoom);

            obj->resetRenderQuality();
            break;
        }

        //Pressing N raises the exponent in Z^n + C by deltaExp, P lowers it by deltaExp
        case GLFW_KEY_N:
        case GLFW_KEY_P:
            if ( key == GLFW_KEY_N || obj->m_Exp >= obj->m_deltaExp)
            {
                obj->m_Exp+= key == GLFW_KEY_N ? obj->m_deltaExp : -obj->m_deltaExp;
                int shader = obj->m_currentShader;
                if (obj->m_Exp == 2.0f)
                {
                    obj->m_currentShader = obj->m_renderJulia ? SHADER_JULIA : SHADER_MANDELBROT;
                    obj->m_guiWindowPos.y += 88.0f;
                    ImGui::SetWindowPos("Mandelbrot", obj->m_guiWindowPos);
                }
                else if (std::abs(obj->m_Exp - 2.0) < .0005)
                {
                    obj->m_Exp = 2.0f;
                    obj->m_currentShader = obj->m_renderJulia ? SHADER_JULIA : SHADER_MANDELBROT;
                    obj->m_guiWindowPos.y += 88.0f;
                    ImGui::SetWindowPos("Mandelbrot", obj->m_guiWindowPos);
                }
                else
                {
                    obj->m_currentShader = obj->m_renderJulia ? SHADER_MULTIJULIA : SHADER_MULTIBROT;
                    obj->m_Shaders[obj->m_currentShader].SetUniform1f("u_Exp", obj->m_Exp);
                }
                if (obj->m_currentShader != shader)
                {
                    obj->m_guiWindowPos.y -= 44.0f;
                    ImGui::SetWindowPos("Mandelbrot", obj->m_guiWindowPos);
                    obj->m_Shaders[obj->m_currentShader].SetUniform2f("u_offset", obj->m_offset.x, obj->m_offset.y);
                    obj->m_Shaders[obj->m_currentShader].SetUniform1f("u_zoom", obj->m_zoom);
                    if (obj->m_currentShader == SHADER_JULIA || obj->m_currentShader == SHADER_MULTIJULIA)
                    {
                        float cx = obj->crosshairMandelCoordX();
                        float cy = obj->crosshairMandelCoordY();
                        obj->m_Shaders[obj->m_currentShader].SetUniform2f("u_cVals", cx, cy);
                    }
                }
                obj->resetRenderQuality();
            }
            break;

        case GLFW_KEY_PERIOD: //Change how much the exponent of our function increments by
            if (obj->m_deltaExp == 0.01f)
                obj->m_deltaExp = 0.05f;
            else if (obj->m_deltaExp == 0.05f)
                obj->m_deltaExp = 0.25f;
            else if (obj->m_deltaExp == 0.25f)
                obj->m_deltaExp = 1.0f;
            else if (obj->m_deltaExp == 1.0f)
                obj->m_deltaExp = 0.01f;
            else
                obj->m_deltaExp = 0.01f;
            break;

        case GLFW_KEY_I: //Toggie GUI
            obj->m_showGui = !obj->m_showGui;
            break;

        case GLFW_KEY_T: //Show how long it took to render the frame. Disabled by default
            obj->m_showRenderTime = !obj->m_showRenderTime;
            obj->m_guiWindowPos.y += obj->m_showRenderTime ? -22.0f : 22.0f;
            ImGui::SetWindowPos("Mandelbrot", obj->m_guiWindowPos);
            break;

        case GLFW_KEY_C: //Re-center the screen at the crosshair
            obj->setOffset();
            obj->m_offsetMandel.x = obj->crosshairMandelCoordX();
            obj->m_offsetMandel.y = obj->crosshairMandelCoordY();
            obj->resetCrosshair();
            obj->resetCrosshairMandel();
            obj->m_imgChanged = true;
            obj->m_scaleFactor = 2;
            obj->m_maxIter = obj->m_maxIterMax;
            break;

        case GLFW_KEY_R: //Reset to center screen / 1x zoom, but don't reset mandelbrot variables
            obj->resetOffset();
            obj->resetCrosshair();
            obj->m_scaleFactor = 2;
            obj->m_imgChanged = true;
            obj->m_maxIter = obj->m_maxIterMax;
            obj->m_zoom = 1.0f;
            obj->m_Shaders[obj->m_currentShader].SetUniform1f("u_zoom", obj->m_zoom);
            break;

        case GLFW_KEY_ESCAPE: //Closes the window and exits the program
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            ImGui::SetWindowPos("Mandelbrot", ImVec2(1655.0f, 1020.0f));
            break;
        }
    }
}

/*generateBuffers generates a grid of (rows * cols) tiles to render to. We will check for user input after each
  tile is rendered -- more tiles is more responsive to input but ultimately slower as it's checking more often*/
void Fractal::generateBuffers(float* buffer, unsigned int* indexes, int rows, int cols, float screenWidth, float screenHeight)
{
    rows++; cols++;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (c < cols - 1 && r < rows - 1)
            {
                int indexPos = r * cols + c;
                int indexBufferPos = (r * (cols-1) + c) * 6;
                indexes[indexBufferPos] = indexPos;
                indexes[indexBufferPos + 1] = (r + 1) * cols + c;
                indexes[indexBufferPos + 2] = indexes[indexBufferPos + 1] + 1;
                indexes[indexBufferPos + 3] = indexPos;
                indexes[indexBufferPos + 4] = indexPos + 1;
                indexes[indexBufferPos + 5] = indexes[indexBufferPos + 2];
                //std::cout << indexBufferPos << "(" << indexes[indexBufferPos] << "," << indexes[indexBufferPos + 1] << "," << indexes[indexBufferPos + 2] << " , "
                //   << indexes[indexBufferPos + 3] << "," << indexes[indexBufferPos + 4] << "," << indexes[indexBufferPos + 5] << ") ";
            }
            int indexPos = r * cols + c;
            int bufferPos = indexPos * 4;
            buffer[bufferPos] = (c / float(cols-1)) * screenWidth;
            buffer[bufferPos + 1] = (r / float(rows-1)) * screenHeight;
            buffer[bufferPos + 2] = c / float(cols-1);
            buffer[bufferPos + 3] = r / float(rows-1);
            //std::cout << bufferPos << "(" << buffer[bufferPos] << ", " << buffer[bufferPos + 1] << ", "
            //    << buffer[bufferPos + 2] << ", " << buffer[bufferPos + 3] << ") ";
        }
        //Uncomment the lines above to see the grids generated by this loop for either the index or vertex buffer
        //std::cout << std::endl;
    }
}
