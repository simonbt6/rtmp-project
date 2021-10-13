#pragma once
/**
 * @author Simon Brisebois-Therrien
 * @date 2021-09-29
 */

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "WindowCallbacks.hpp"
#include "Render.hpp"

#include <linmath.h>
#include <iostream>

#include <FileManager.hpp>
#include <Memory.hpp>

namespace Graphics
{
    class Window
    {
        private:
            GLFWwindow *m_Window;

            Render* m_Render;

            std::vector<uint8_t>* frame_data;

            int m_Window_width, m_Window_height;

        public:
            void Initialize();

            Render* GetRender() const { return m_Render; }

        private:

            void Loop();
            
            void CleanUp();

    };
};