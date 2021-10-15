#include "Renderer2D.hpp"

namespace Graphics
{
    Renderer2D::Renderer2D()
    {
        this->LoadShaders();
    }

    Renderer2D::~Renderer2D()
    {
        
    }

    void Renderer2D::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer2D::Draw(const IndexBuffer& ibo, const Shader& shader)
    {
        shader.Bind();
        m_VAO.Bind();
        ibo.Bind();

        glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer2D::Draw(const IRenderable2D& renderable)
    {
        // Draw bounds
        DrawRect(renderable.GetBounds(), renderable.GetColor());
        VertexBuffer vbo(renderable.GetVerticesArray(), renderable.GetVertexCount() * 4 * sizeof(float));
        IndexBuffer ibo(renderable.GetIndices(), renderable.GetIndexCount());

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO.AddBuffer(vbo, layout);
        
        Shader* shader = GetShader("texture");
        shader->Bind();
        try
        {
            const Texture& texture = renderable.GetTexture();
            texture.Bind();
            shader->SetUniform1i("u_Texture", 0);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        Draw(ibo, *shader);
        
    }

    void Renderer2D::DrawRect(const Maths::Rectangle& rectangle, const Maths::Color& color)
    {
        const Maths::vec2<float>& position = rectangle.GetPosition();
        const Maths::vec2<float>& size = rectangle.GetSize();

        const float width = size.GetX(), 
                    height = size.GetY(),
                    x = position.GetX(),
                    y = position.GetY();

        float vertices[] = {
            (-width + x), (-height + y), 
            ( width + x), (-height + y),
            ( width + x),  (height + y),
            (-width + x),  (height + y)
        };
        uint32_t indices[6] = { 0, 1, 2,    2, 3, 0 };

        IndexBuffer  ibo(indices,  6);
        VertexBuffer vbo(vertices, 8 * sizeof(float)); 

        VertexBufferLayout layout;
        layout.Push<float>(2);
        m_VAO.AddBuffer(vbo, layout);

        Shader* shader = GetShader("basic");
        shader->Bind();
        shader->SetUniform4f("u_Color", color.GetR(), color.GetG(), color.GetB(), color.GetA());

        Draw(ibo, *shader);
    }

    void Renderer2D::DrawRect(float width, float height, Maths::vec4<float> color)
    {
        Clear();
        float vertices[] = {
        //  Position        Couleur             Coordonnées de texture
            -width, -height,   0.0f, 0.0f, // Haut gauche
             width, -height,   1.0f, 0.0f, // Haut droit
             width,  height,   1.0f, 1.0f, // Bas droit
            -width,  height,   0.0f, 1.0f  // Bas gauche
        };

        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_VAO.Bind();
        VertexBuffer vbo (vertices, sizeof(vertices));
        IndexBuffer  ibo (indices, 6);

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO.AddBuffer(vbo, layout);

        Shader* shader = GetShader("texture");
        shader->Bind();

        Texture texture("lilo.jpg");
        texture.Bind();
        shader->SetUniform1i("u_Texture", 0);

        Draw(ibo, *shader);


    }

    void Renderer2D::DrawSprite(const Texture& texture, float width, float height)
    {
        Clear();
        float vertices[] = {
        //  Position        Couleur             Coordonnées de texture
            -width, -height,   0.0f, 0.0f, // Haut gauche
             width, -height,   1.0f, 0.0f, // Haut droit
             width,  height,   1.0f, 1.0f, // Bas droit
            -width,  height,   0.0f, 1.0f  // Bas gauche
        };

        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        m_VAO.Bind();
        
        
        VertexBuffer vbo (vertices, sizeof(vertices));
        IndexBuffer  ibo (indices, 6);

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO.AddBuffer(vbo, layout);

        Shader* shader = GetShader("texture");
        shader->Bind();

        texture.Bind();
        shader->SetUniform1i("u_Texture", 0);

        Draw(ibo, *shader);
    }


    Shader* Renderer2D::GetShader(const std::string& name)
    {
        return 
            m_Shaders.contains(name)? 
            m_Shaders.at(name): 
            throw std::runtime_error((std::string("\nCould not find ") + name + std::string(" shader.").c_str()));
    }

    void Renderer2D::LoadShaders()
    {
        for (const std::string& shader_name : *Utils::FileManager::ReadLinesFromFile(s_FolderPath + "shaders"))
            m_Shaders.insert(std::pair<std::string, Shader*> (shader_name, new Shader(shader_name)));
    }
}