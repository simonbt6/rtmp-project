#pragma once


#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <FormatedPrint.hpp>



namespace Graphics 
{
    class WindowCallbacks
    {
        public:
            static void WindowResizeCallback(GLFWwindow* window, int width, int height);

            static void ErrorCallback(int error_code, const char* error_message);

            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

            static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    };
};