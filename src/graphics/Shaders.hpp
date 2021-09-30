#pragma once
/**
 * @author Simon Brisebois-Therrien
 * @date 2021-09-30 
 * 
 * 
 */

#include <iostream>
#include <vector>
#include <string>

#include <FileManager.hpp>
#include <FormatedPrint.hpp>

namespace Graphics
{
    typedef struct ShaderSource
    {
        std::string vertex_src;
        std::string frag_src;
    };

    typedef struct Shader
    {

    };

    class Shaders
    {
        private:
            std::string src_path = "D:/dev-repo/VideoStreaming/RTMP/src/graphics/shaders/";
            std::vector<std::string>* m_Shader_names;

        public:
            Shaders();

            void LoadShaders();
            void CompileShaders();

        private:
            void LoadShader(std::string name);
            void CompileShader(ShaderSource shader_src);
    }; 
};

