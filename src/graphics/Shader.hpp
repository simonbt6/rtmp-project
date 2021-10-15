#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-12 
 * 
 * 
 */
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "utils/vec2.hpp"
#include "utils/vec3.hpp"
#include "utils/vec4.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <inttypes.h>

#include <FileManager.hpp>
#include <FormatedPrint.hpp>

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

            /**
             * UNIFORM 1
             * 
             */
            // Float
            void SetUniform1f(const std::string& name, float v0);

            // Int32
            void SetUniform1i(const std::string& name, int32_t v0);
            
            /**
             * UNIFORM 2
             * 
             */
            // Float
            void SetUniform2f(const std::string& name, Maths::vec2<float> v);
            void SetUniform2f(const std::string& name, float v0, float v1);

            // Int32
            void SetUniform2i(const std::string& name, Maths::vec2<int32_t> v);
            void SetUniform2i(const std::string& name, int32_t v0, int32_t v1);

            /**
             * UNIFORM 3
             * 
             */
            // Float
            void SetUniform3f(const std::string& name, Maths::vec3<float> v);
            void SetUniform3f(const std::string& name, float v0, float v1, float v2);

            // Int32
            void SetUniform3i(const std::string& name, Maths::vec3<int32_t> v);
            void SetUniform3i(const std::string& name, int32_t v0, int32_t v1, int32_t v2);
            
            /**
             * UNIFORM 4
             * 
             */
            // Float
            void SetUniform4f(const std::string& name, Maths::vec4<float> v);
            void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

            // Int32
            void SetUniform4i(const std::string& name, Maths::vec4<int32_t> v);
            void SetUniform4i(const std::string& name, int32_t v0, int32_t v1, int32_t v2, int32_t v3);


            std::string GetFilepath() { return s_FolderPath + m_Name; };

        private:
            
            void LoadShader(const std::string& name);
            void CreateShader();
            uint32_t CompileShader(uint32_t type, const char* src);

            int32_t GetUniformLocation(const std::string& name);

    };
}