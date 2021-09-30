#include "WindowCallbacks.hpp"


namespace Graphics
{
    void WindowCallbacks::WindowResizeCallback(GLFWwindow* window, int width, int height)
    {
        Utils::FormatedPrint::PrintInfo("Window resized.");
        glfwSetWindowTitle(window, (std::to_string(width) + string(" / ") + std::to_string(height)).c_str());
        glViewport(0, 0, width, height);
    }
            
    void WindowCallbacks::ErrorCallback(int error_code, const char* error_message)
    {
        throw std::runtime_error(error_message);
    }

    void WindowCallbacks::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        const char* keyname = glfwGetKeyName(key, scancode);
        if (keyname == nullptr || keyname == "")
            return Utils::FormatedPrint::PrintError("WindowCallbacks::KeyCallback", "Unknown key name.");
        Utils::FormatedPrint::PrintInfo("Key pressed [" + string(keyname) + "].");
    }

    void WindowCallbacks::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
    {
        Utils::FormatedPrint::PrintInfo("Mouse button event.");
    }
};