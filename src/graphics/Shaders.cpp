#include "Shaders.hpp"

namespace Graphics
{
    Shaders::Shaders()
    {

    }
    
    void Shaders::LoadShaders()
    {
        this->m_Shader_names = Utils::FileManager::ReadLinesFromFile(src_path + "shaders");

        if (this->m_Shader_names->empty())
            throw std::runtime_error("\nCould not load shaders.");

        for (string& name : *m_Shader_names)
        {
            Utils::FormatedPrint::PrintFormated("Shaders::LoadShaders", "Loading shader -> " + name, Utils::FormatedPrint::Color::Yellow);
            LoadShader(name);
            Utils::FormatedPrint::PrintFormated("Shaders::LoadShaders", "Shader loaded -> " + name, Utils::FormatedPrint::Color::Green);
        }
    }

    void Shaders::LoadShader(std::string name)
    {
        ShaderSource shader_src;

        // Load vertex shader.
        if((shader_src.vertex_src = Utils::FileManager::ReadStringFromFile(src_path + name + ".vert")) == "")
            throw std::runtime_error("\nCould not open [" + name + ".vert].");

        // Load frag shader.
        if((shader_src.frag_src = Utils::FileManager::ReadStringFromFile(src_path + name + ".frag")) == "")
            throw std::runtime_error("\nCould not open [" + name + ".frag].");
    }
};