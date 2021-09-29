#pragma once
/**
 * @author Simon Brisebois-Therrien
 * @date 2021-09-29
 */

#include <iostream>
#include <GLFW/glfw3.h>

namespace Graphics
{
    class Window
    {
        private:
            GLFWwindow *m_Window;

        public:
            void Initialize();

        private:

            void Loop();
            
            void CleanUp();
    };
};