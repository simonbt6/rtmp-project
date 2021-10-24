#include "Window.hpp"

namespace Graphics
{

    void Window::Initialize()
    {
        if (!glfwInit())
            throw std::runtime_error("Could not initialize GLFW.");
        
        this->m_Window = glfwCreateWindow(720, 540, "RTMP Graphics.", NULL, NULL);

        if (!this->m_Window)
            throw std::runtime_error("Could not open window.");

        printf("GLFW Version: %s\n", glfwGetVersionString());

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
        this->m_Renderer2D = new Renderer2D();
        
    };


    void Window::CleanUp()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::Loop(void(* fn)(Graphics::Renderer2D&))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        Maths::vec2<float> pos(0.5f, -0.5f);
        Maths::vec2<float> size(0.5f, 0.5f);
        Color              color(0.0f, 1.0f, 0.0f, 1.0f);

        Primitives::Sprite sprite(pos, size, color);

        while (!glfwWindowShouldClose(this->m_Window))
        {
            m_Render->Clear();
            float ratio;
            glfwGetFramebufferSize(m_Window, &m_Window_width, &m_Window_height);
            ratio = m_Window_width / m_Window_height;

            // glViewport(0, 0, m_Window_width, m_Window_height);

            if (fn != nullptr) fn(*m_Renderer2D);


            // m_Render->DrawRectangle(400, 400, 0x3F, 0xFF, 0x7F);
            // m_Render->DrawFrame(frame_data->data(), frame_data->size(), 1.0f, 1.0f);
            // m_Renderer2D->DrawRectangle(0.5f, 0.5f, Maths::vec4<float>(1.0f, 0.0f, 0.0f, 1.0f));
            // m_Renderer2D->DrawSprite(Texture("frame-6.pgm"), 1.0f, 1.0f);
            // m_Renderer2D->DrawRect(Maths::Rectangle(0.5f, 0.5f, 0.5f, 0.5f), Color(1.0f, 0.0f, 0.0f, 1.0f));
            // m_Renderer2D->Draw(sprite);
            glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }   

        this->CleanUp();
    }
};