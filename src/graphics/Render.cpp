#include "Render.hpp"

namespace Graphics
{

    void Render::Clear() const
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Render::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
    {
        shader.Bind();
        vao.Bind();
        ibo.Bind();

        glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);

        shader.Unbind();
        vao.Unbind();
        ibo.Unbind();
    }



    void Render::DrawTriangle(float vertices[], char r, char g, char b)
    {
        Shader* shader = GetShader("triangle");

        VertexBuffer vbo(vertices, 6 * sizeof(float));

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

        VertexBuffer vbo(rectangle_bounds_vertices, 8 * sizeof(float));

        m_VAO.Bind();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

        IndexBuffer ibo(rectangle_bounds_indices, 6);
        

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, rectangle_bounds_indices);

        shader->Unbind();
        vbo.Unbind();
    }

    void Render::DrawFrame(uint8_t* bytes, int size, float width, float height)
    {
        float max = 0.5f;
        
        // Setup viewport
        // DrawRectangle(width, height, 0, 0, 0);
        
        float vertices[] = {
            // Position          Tex Coords
            -width, -height,     0.0f,  0.0f,
             width, -height,     1.0f,  0.0f,
             width,  height,     1.0f,  1.0f,
            -width,  height,     0.0f,  1.0f,
        };

        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        VertexArray  vao;
        VertexBuffer vbo(vertices, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        vao.AddBuffer(vbo, layout);

        IndexBuffer ibo(indices, 6);

        Shader* shader = GetShader("texture");
        shader->Bind();

        Texture texture("mat.jpg");
        texture.Bind();
        shader->SetUniform1i("u_Texture", 0);

        vao.Unbind();
        vbo.Unbind();
        ibo.Unbind();
        shader->Unbind();

        texture.Bind();
        Draw(vao, ibo, *shader);


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