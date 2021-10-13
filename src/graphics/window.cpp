#include "Window.hpp"

namespace Graphics
{

    void Window::Initialize()
    {
        if (!glfwInit())
            throw std::runtime_error("Could not initialize GLFW.");
        
        this->m_Window = glfwCreateWindow(640, 480, "RTMP Server.", NULL, NULL);

        if (!this->m_Window)
            throw std::runtime_error("Could not open window.");

        printf("GLFW Version: %s\n", glfwGetVersionString());
        frame_data = Utils::FileManager::ReadBinaryFile("frame-6.pgm");
        Utils::FormatedPrint::PrintInfo("Frame data size: " + std::to_string(frame_data->size()));

        glfwSetErrorCallback(WindowCallbacks::ErrorCallback);
        glfwSetFramebufferSizeCallback(m_Window, WindowCallbacks::WindowResizeCallback);
        glfwSetKeyCallback(m_Window, WindowCallbacks::KeyCallback);
        glfwSetMouseButtonCallback(m_Window, WindowCallbacks::MouseButtonCallback);

        glfwMakeContextCurrent(m_Window);
        gladLoadGL(glfwGetProcAddress);
        glfwSwapInterval(1);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0.0f, 400.0f, 0.0f, 400.0f, 0.0f, 1.0f);

        this->m_Render = new Render(m_Window);
        this->Loop();
        this->CleanUp();

        
    };


    void Window::CleanUp()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::Loop()
    {
        while (!glfwWindowShouldClose(this->m_Window))
        {
            float ratio;
            glfwGetFramebufferSize(m_Window, &m_Window_width, &m_Window_height);
            ratio = m_Window_width / m_Window_height;

            glViewport(0, 0, m_Window_width, m_Window_height);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);


            // m_Render->DrawRectangle(400, 400, 0x3F, 0xFF, 0x7F);
            m_Render->DrawFrame(frame_data->data(), frame_data->size(), 0.8f, 0.5f);
            
            glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }   
    }
};