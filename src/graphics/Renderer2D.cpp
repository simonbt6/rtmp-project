#include "Renderer2D.hpp"

namespace Graphics
{
    Renderer2D::Renderer2D()
    {
        this->LoadShaders();
        this->LoadCharacters();
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

    void Renderer2D::DrawRect(const Maths::Rectangle& rectangle, const Color& color)
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


    void Renderer2D::DrawText(Maths::vec2<float> position, float scale, const std::string& text, const Color& color)
    {
        Shader* shader = GetShader("TextShader");
        shader->SetUniform4f("textColor", color);

        // Maths::mat4<float> id = Maths::mat4<float>::Identity();

        glActiveTexture(GL_TEXTURE0);
        shader->SetUniform1i("text", 0);
        m_VAO.Bind();

        std::string::const_iterator it;
        for (it = text.begin(); it != text.end(); it++)
        {
            TextCharacter c = m_Characters[*it];

            float xpos = position.GetX() + c.Bearing.GetX() * scale;
            float ypos = position.GetY() + c.Bearing.GetY() * scale;

            float width = c.Bearing.GetX() * scale;
            float height = c.Bearing.GetY() * scale;

            float vertices[6][4] = {
                {xpos,         ypos + height, 0.0f, 0.0f},
                {xpos,         ypos,          0.0f, 1.0f},
                {xpos + width, ypos,          1.0f, 1.0f,},
                
                {xpos,         ypos + height, 0.0f, 0.0f,},
                {xpos + width, ypos,          1.0f, 1.0f,},
                {xpos + width, ypos + height, 1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, c.TextureID);
            VertexBuffer vbo(0, 0);
            vbo.Bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            vbo.Unbind();
            
            glDrawArrays(GL_TRIANGLES, 0, 6);

            position += Maths::vec2<float>((c.Advance >> 6) * scale, 0.0f);
        }
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

    void Renderer2D::LoadCharacters()
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) throw std::runtime_error("\nFailed to init FreeType library.");

        FT_Face face;
        if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) throw std::runtime_error("\nFailed to load font.");

        FT_Set_Pixel_Sizes(face, 0, 48);

        if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) throw std::runtime_error("\nFailed to load Glyph.");

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (uint8_t c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::runtime_error("\nFailed to load Glyph");
                continue;
            }

            uint32_t texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            TextCharacter character = {
                texture,
                Maths::vec2<float>(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                Maths::vec2<float>(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            m_Characters.insert(std::pair<char, TextCharacter>(c, character));
        }
    }
}