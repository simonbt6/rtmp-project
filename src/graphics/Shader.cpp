#include "Shader.hpp"

namespace Graphics
{

    Shader::Shader(const std::string& name)
    {
        this->m_Name = name;
        this->LoadShader(name);
        this->CreateShader();
    }

    Shader::~Shader()
    {

    }

    void Shader::LoadShader(const std::string& name)
    {
        if((this->m_ShaderSource.frag_src = Utils::FileManager::ReadStringFromFile(s_FolderPath + name + s_FragExt)) == "")
            throw std::runtime_error("\nCould not read shader fragment source file.");
 
        if((this->m_ShaderSource.vertex_src = Utils::FileManager::ReadStringFromFile(s_FolderPath + name + s_VertexExt)) == "")
            throw std::runtime_error("\nCould not read shader vertex source file.");
    }

    void Shader::CreateShader()
    {
        this->m_RenderID = glCreateProgram();

        const char* vertex_source = this->m_ShaderSource.vertex_src.c_str();
        const char* frag_source = this->m_ShaderSource.frag_src.c_str();

        uint32_t vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_source);
        uint32_t frag_shader   = CompileShader(GL_FRAGMENT_SHADER, frag_source);

        glAttachShader(this->m_RenderID, vertex_shader);
        glAttachShader(this->m_RenderID, frag_shader);
        glLinkProgram(this->m_RenderID);

        glDeleteShader(vertex_shader);
        glDeleteShader(frag_shader);

        if (!frag_shader || !vertex_shader)
            throw std::runtime_error((std::string("\nCould not create shader ") + m_Name + std::string(".")).c_str());

        Utils::FormatedPrint::PrintInfo("Finished loading " + m_Name + std::string(" shader."));
    }

    uint32_t Shader::CompileShader(uint32_t type, const char* src)
    {
        uint32_t shader = glCreateShader(type);

        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            char * message = (char*) alloca(length * sizeof(char));
            glGetShaderInfoLog(shader, length, &length, message);
            throw std::runtime_error(message);
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

    void Shader::Bind() const
    {
        glUseProgram(m_RenderID);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    int32_t Shader::GetUniformLocation(const std::string& name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        int32_t location = glGetUniformLocation(m_RenderID, name.c_str());
        if (location == -1)
            Utils::FormatedPrint::PrintInfo("Warning: uniform '" + name + std::string("' doesn't exist."));
        
        m_UniformLocationCache[name] = location;
        return location;
    }

    void Shader::SetUniform1f(const std::string& name, float v0)
    {
        glUniform1f(GetUniformLocation(name), v0);
    }

    void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
    {
        glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
    }
};