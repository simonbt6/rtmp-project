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
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"

#include "utils/vec2.hpp"
#include "utils/vec4.hpp"

#include <iostream>
#include <linmath.h>
#include <map>


namespace Graphics
{
    class Render
    {
        private:
            GLFWwindow* m_Window;

            VertexArray m_VAO;

            std::map<std::string, Shader*> m_Shaders;

            static inline const std::string s_FolderPath = "D:/dev-repo/VideoStreaming/RTMP/src/graphics/shaders/";


        public:

            Render(GLFWwindow* window)
            {
                this->m_Window = window;

                this->LoadShaders();
                
            }
            typedef struct
            {
                float x, y;
                float r, g, b;
            } Vertice;

            void Clear() const;

            void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;

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