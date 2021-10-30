#include "Renderer2D.hpp"

namespace Graphics
{
    Renderer2D::Renderer2D(Maths::mat4 projectionMatrix)
        : m_TextRenderer(new TextRenderer()), m_ProjectionMatrix(projectionMatrix)
    {
        this->LoadShaders();
        // this->LoadCharacters();
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
        const Maths::vec2& position = rectangle.GetPosition();
        const Maths::vec2& size = rectangle.GetSize();

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

    void Renderer2D::DrawRect(float width, float height, const Color& color)
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

    void Renderer2D::DrawSprite(const Texture& texture, Maths::Rectangle rectangle)
    {
        DrawSprite(texture, rectangle.GetPosition(), rectangle.GetSize());
    }

    void Renderer2D::DrawSprite(const Texture& texture, Maths::vec2 position, Maths::vec2 size)
    {
        DrawSprite(texture, position.GetX(), position.GetY(), size.GetX(), size.GetY());
    }

    void Renderer2D::DrawSprite(const Texture& texture, float x, float y, float width, float height)
    {
        Clear();
        float vertices[] = {
        //  Position        Couleur             Coordonnées de texture
            (-width + x), (-height + y),   0.0f, 0.0f, // Haut gauche
            ( width + x), (-height + y),   1.0f, 0.0f, // Haut droit
            ( width + x), ( height + y),   1.0f, 1.0f, // Bas droit
            (-width + x), ( height + y),   0.0f, 1.0f  // Bas gauche
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
        glMatrixMode(GL_PROJECTION);

        texture.Bind();
        
        Maths::mat4 view(1);
        Maths::mat4 model(1);
        Maths::mat4 proj = Maths::mat4::Translate(Maths::vec3(0.0, 0, 0));

        Maths::mat4 mvp = proj * view * model;
        shader->SetUniformMat4("u_Projection", mvp);
        shader->SetUniform1i("u_Texture", 0);

        Draw(ibo, *shader);
    }

    void Renderer2D::DrawText(const std::string& text, const std::string& font_name, float size, const Color& color, Maths::vec2 position)
    {
        m_TextRenderer->DrawString(text, font_name, color, size, position);
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

    // void Renderer2D::LoadCharacters()
    // {
    //     FT_Library ft;
    //     if (FT_Init_FreeType(&ft)) throw std::runtime_error("\nFailed to init FreeType library.");

    //     FT_Face face;
    //     if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) throw std::runtime_error("\nFailed to load font.");

    //     FT_Set_Pixel_Sizes(face, 0, 48);

    //     if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) throw std::runtime_error("\nFailed to load Glyph.");

    //     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //     for (uint8_t c = 0; c < 128; c++)
    //     {
    //         if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    //         {
    //             std::runtime_error("\nFailed to load Glyph");
    //             continue;
    //         }

    //         uint32_t texture;
    //         glGenTextures(1, &texture);
    //         glBindTexture(GL_TEXTURE_2D, texture);
    //         glTexImage2D(
    //             GL_TEXTURE_2D,
    //             0,
    //             GL_RED,
    //             face->glyph->bitmap.width,
    //             face->glyph->bitmap.rows,
    //             0,
    //             GL_RED,
    //             GL_UNSIGNED_BYTE,
    //             face->glyph->bitmap.buffer
    //         );
    //         glEnable(GL_BLEND);
    //         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //         TextCharacter character = {
    //             texture,
    //             Maths::vec2<float>(face->glyph->bitmap.width, face->glyph->bitmap.rows),
    //             Maths::vec2<float>(face->glyph->bitmap_left, face->glyph->bitmap_top),
    //             (uint32_t)face->glyph->advance.x
    //         };
    //         m_Characters.insert(std::pair<char, TextCharacter>(c, character));
    //     }
    // }
}