#include "Render.hpp"

namespace Graphics
{

    void Render::DrawTriangle(float vertices[], char r, char g, char b)
    {
        Shader* shader = GetShader("triangle");

        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

        shader->Bind();
        shader->SetUniform4f("color", r/255.0f, g/255.0f, b/255.0f, 1.0f);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        shader->Unbind();
        
    }

    void Render::DrawRectangle(float width, float height, char r, char g, char b)
    {

        float rectangle_bounds_vertices[] = {
            -width,  height,
            -width, -height,
             width, -height,
             width,  height
        };
        uint32_t rectangle_bounds_indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        Shader* shader = GetShader("basic");
        shader->Bind();
        shader->SetUniform4f("color", r, g, b, 1.0f);
    }

    void Render::DrawFrame(uint8_t* bytes, int size, float width, float height)
    {
        float max = 0.5f;
        
    
        

    }

    Shader* Render::GetShader(const std::string& name)
    {
        return 
            m_Shaders.contains(name)? 
            m_Shaders.at(name): 
            throw std::runtime_error((std::string("\nCould not find ") + name + std::string(" shader.").c_str()));
    }

    void Render::LoadShaders()
    {
        for (const std::string& shader_name : *Utils::FileManager::ReadLinesFromFile(s_FolderPath + "shaders"))
            m_Shaders.insert(std::pair<std::string, Shader*> (shader_name, new Shader(shader_name)));
    }
};