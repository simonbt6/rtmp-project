#pragma once
/**
 * @author Simon Brisebois-Therrien
 * @date 2021-09-29 
 */
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "WindowCallbacks.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

#include <iostream>
#include <linmath.h>
#include <map>
#include "utils/vec2.hpp"


namespace Graphics
{
    class Render
    {
        private:
            GLFWwindow* m_Window;

            uint32_t m_VAO;
            uint32_t m_Buffer;

            std::map<std::string, Shader*> m_Shaders;

            static inline const std::string s_FolderPath = "D:/dev-repo/VideoStreaming/RTMP/src/graphics/shaders/";


        public:

            Render(GLFWwindow* window)
            {
                this->m_Window = window;

                this->LoadShaders();
                

                glGenBuffers(1, &m_Buffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
            }
            typedef struct
            {
                float x, y;
                float r, g, b;
            } Vertice;

            void DrawTriangle(float vertices[], char r, char g, char b);

            void DrawRectangle(float width, float height, char r, char g, char b);

            void DrawFrame(uint8_t* bytes, int size, float height, float width);

            Shader* GetShader(const std::string& name);

        private:


            void LoadShaders();
            
            void SetupVAO();

            void SetupBuffer();
    };
};