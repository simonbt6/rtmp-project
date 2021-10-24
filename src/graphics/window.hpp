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
#include "color.hpp"

#include <maths/vec2.hpp>
#include <maths/vec3.hpp>
#include <maths/vec4.hpp>
#include <maths/rectangle.hpp>

#include "Renderables/Sprite.hpp"

#include <linmath.h>
#include <iostream>
#include <stack>

#include <utils/FileManager.hpp>

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

            void Loop(void(* fn)(Graphics::Renderer2D&));

            Render* GetRender() const { return m_Render; }


        private:
            
            void CleanUp();

    };
};