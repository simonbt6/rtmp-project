#include "window.hpp"

namespace Graphics
{
    void Window::Initialize()
    {
        if (!glfwInit())
            throw std::runtime_error("Could not initialize GLFW.");
        
        this->m_Window = glfwCreateWindow(640, 480, "RTMP Server.", NULL, NULL);

        if (!this->m_Window)
            throw std::runtime_error("Could not open window.");

        this->Loop();

        
    };

    unsigned char* buffer = new unsigned char[100 * 100 * 3];

    void Window::Loop()
    {
        for (unsigned int y = 0; y < 100; ++y)
            for (unsigned int x = 0; x < 100; ++x)
            {
                buffer[y * 100 * 3 + x * 3    ] = 0xFF;
                buffer[y * 100 * 3 + x * 3 + 1] = 0x00;
                buffer[y * 100 * 3 + x * 3 + 2] = 0x00;

                
            }
        while (!glfwWindowShouldClose(this->m_Window))
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDrawPixels(100, 100, GL_RGB, GL_UNSIGNED_BYTE, buffer);
            glfwSwapBuffers(m_Window);

            glfwPollEvents();
        }
    }
};