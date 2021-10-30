#include "Shaders.hpp"

namespace Graphics
{
    Shaders::Shaders()
    {
        // Initialize shader vectors.
        this->m_Shaders_src = new vector<ShaderSource*>();
        this->m_Shaders = new map<std::string, Shader*>();
    }
    
    void Shaders::LoadShaders()
    {
        this->m_Shader_names = Utils::FileManager::ReadLinesFromFile(src_path + "shaders");

        if (this->m_Shader_names->empty())
            throw std::runtime_error("\nCould not load shaders.");
        Utils::FormatedPrint::PrintFormated("Shaders::LoadShaders", "Loading shaders.");
        for (string& name : *m_Shader_names)
        {
            Utils::FormatedPrint::PrintFormated("Shaders::LoadShaders", "Loading shader -> " + name, Utils::FormatedPrint::Color::Yellow);
            LoadShader(name);
            Utils::FormatedPrint::PrintFormated("Shaders::LoadShaders", "Shader loaded successfully -> " + name, Utils::FormatedPrint::Color::Green);
        }
    }

    void Shaders::CompileShaders()
    {
        Utils::FormatedPrint::PrintFormated("Shaders::CompileShaders", "Compiling shaders.");
        for (ShaderSource* shader_src : *m_Shaders_src)
        {
            Utils::FormatedPrint::PrintFormated("Shaders::CompileShaders", "Compiling shader -> " + shader_src->name, Utils::FormatedPrint::Color::Yellow);
            CompileShader(shader_src);
            Utils::FormatedPrint::PrintFormated("Shaders::CompileShaders", "Shader compiled successfully -> " + shader_src->name, Utils::FormatedPrint::Color::Green);
        }
    }

    Shader* Shaders::GetShader(std::string name)
    {
        if (m_Shaders->contains(name))
            return m_Shaders->at(name);
        else
            return nullptr;
    }

    void Shaders::LoadShader(std::string name)
    {
        ShaderSource* shader_src = new ShaderSource();

        // Load vertex shader.
        std::string vertex_src = Utils::FileManager::ReadStringFromFile(src_path + name + ".vert");
        if(vertex_src == "")
            throw std::runtime_error("\nCould not open [" + name + ".vert].");
        shader_src->vertex_src = vertex_src.c_str();
        printf("\n%s", shader_src->vertex_src);

        // Load frag shader.
        std::string frag_src = Utils::FileManager::ReadStringFromFile(src_path + name + ".frag");
        if(frag_src == "")
            throw std::runtime_error("\nCould not open [" + name + ".frag].");
        shader_src->frag_src = frag_src.c_str();
        printf("\n%s", shader_src->frag_src);


        shader_src->name = name;

        m_Shaders_src->push_back(shader_src);
    }
};