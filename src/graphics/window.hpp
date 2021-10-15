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
#include "Renderer2D.hpp"
#include "Texture.hpp"

#include "utils/vec2.hpp"
#include "utils/vec3.hpp"
#include "utils/vec4.hpp"
#include "utils/color.hpp"
#include "utils/rectangle.hpp"

#include "Renderables/Sprite.hpp"

#include <linmath.h>
#include <iostream>
#include <stack>

#include <FileManager.hpp>

namespace Graphics
{
    class Window
    {
        private:
            GLFWwindow *m_Window;

            Render* m_Render;
            Renderer2D* m_Renderer2D;

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