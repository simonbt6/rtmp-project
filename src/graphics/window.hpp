#pragma once
/**
 * @author Simon Brisebois-Therrien
 * @date 2021-09-29
 */

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "WindowCallbacks.hpp"

#include <maths/maths.hpp>

#include <linmath.h>
#include <iostream>

#include <utils/FileManager.hpp>

namespace Graphics
{
    class Window
    {
        private:
            GLFWwindow *m_GLWindow;
            std::string m_Title;

            Maths::Rectangle m_Bounds;

            bool m_Closed;
            void(* m_Handler)();

        public:
            Window(const std::string& title, const Maths::Rectangle& bounds);
            ~Window(){};

            void Init();

            void SetTitle(const std::string& title);

            inline void SetHandler(void(* handler)(void)) { m_Handler = handler; }

            inline void Start() { Loop(); }
            inline void Stop() { glfwSetWindowShouldClose(m_GLWindow, true); }

            inline const Maths::vec2& GetPosition() const { return m_Bounds.GetPosition(); }
            inline const Maths::vec2& GetSize()     const { return m_Bounds.GetSize(); }
            inline const Maths::Rectangle&   GetBounds()   const { return m_Bounds; }

            inline void SetSize    (const Maths::vec2& size)     { m_Bounds.SetSize(size); }
            inline void SetPosition(const Maths::vec2& position) { m_Bounds.SetPosition(position); }
            inline void SetBounds  (const Maths::Rectangle& bounds)     { m_Bounds = bounds; } 

        private:
            void Loop();
            void CleanUp();


    };
};