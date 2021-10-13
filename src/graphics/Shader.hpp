#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-12 
 * 
 * 
 */
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <inttypes.h>

#include <FileManager.hpp>

namespace Graphics
{
    struct ShaderSource
    {
        std::string name,
                    vertex_src, 
                    frag_src;
    };
    class Shader
    {
        private:
            static inline const std::string s_FolderPath = "D:/dev-repo/VideoStreaming/RTMP/src/graphics/shaders/";
            static inline const std::string s_VertexExt = ".vert";
            static inline const std::string s_FragExt = ".frag";

            std::string m_Name;
            uint32_t m_RenderID;
            std::unordered_map<std::string, int32_t> m_UniformLocationCache;

            ShaderSource m_ShaderSource;

        public:
            Shader(const std::string& name);
            ~Shader();

            void Bind() const;
            void Unbind() const;

            void SetUniform1f(const std::string& name, float v0);
            void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);


            std::string GetFilepath() { return s_FolderPath + m_Name; };

        private:
            
            void LoadShader(const std::string& name);
            void CreateShader();
            uint32_t CompileShader(uint32_t type, const char* src);

            int32_t GetUniformLocation(const std::string& name);

    };
}