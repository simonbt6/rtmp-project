#include "Window.hpp"

namespace Graphics
{

    Window::Window(const std::string& title, const Maths::Rectangle& bounds): m_Title(title), m_Bounds(bounds)
    {

    }

    void Window::Init()
    {
        if (!glfwInit())
            throw std::runtime_error("Could not initialize GLFW.");
        
        this->m_GLWindow = glfwCreateWindow(m_Bounds.GetSize().GetX(), m_Bounds.GetSize().GetY(), m_Title.c_str(), NULL, NULL);

        if (m_Bounds.GetPosition().GetX() != 0 || m_Bounds.GetPosition().GetY() != 0)
            glfwSetWindowPos(m_GLWindow, m_Bounds.GetPosition().GetX(), m_Bounds.GetPosition().GetY());
        else
        {
            int xpos, ypos;
            glfwGetWindowPos(m_GLWindow, &xpos, &ypos);
            m_Bounds.SetPosition(Maths::vec2(xpos, ypos));
        }

        if (!this->m_GLWindow)
            throw std::runtime_error("Could not open window.");

        printf("GLFW Version: %s\n", glfwGetVersionString());

        glfwSetErrorCallback(WindowCallbacks::ErrorCallback);
        glfwSetFramebufferSizeCallback(m_GLWindow, WindowCallbacks::WindowResizeCallback);
        glfwSetWindowPosCallback(m_GLWindow, WindowCallbacks::WindowPositionCallback);
        glfwSetKeyCallback(m_GLWindow, WindowCallbacks::KeyCallback);
        glfwSetMouseButtonCallback(m_GLWindow, WindowCallbacks::MouseButtonCallback);

        glfwMakeContextCurrent(m_GLWindow);
        gladLoadGL(glfwGetProcAddress);
        glfwSwapInterval(1);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0.0f, 400.0f, 0.0f, 400.0f, 0.0f, 1.0f);
        
    };

    void Window::SetTitle(const std::string& title)
    {
        m_Title = title;
        glfwSetWindowTitle(m_GLWindow, m_Title.c_str());
    }


    void Window::CleanUp()
    {
        glfwDestroyWindow(m_GLWindow);
        glfwTerminate();
    }

    void Window::Loop()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        while (!glfwWindowShouldClose(this->m_GLWindow))
        {

            if (m_Handler != nullptr) m_Handler();

            glfwSwapBuffers(m_GLWindow);
            glfwPollEvents();
        }   

        this->CleanUp();
    }
};