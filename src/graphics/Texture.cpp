#define STB_IMAGE_IMPLEMENTATION
#include "Texture.hpp"



namespace Graphics
{
    Texture::Texture(const std::string& filepath)
        : m_RenderID(0), m_Filepath(filepath), m_TextureBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
    {
        stbi_set_flip_vertically_on_load(1);

        m_TextureBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

        glGenTextures(1, &m_RenderID);
        glBindTexture(GL_TEXTURE_2D, m_RenderID);      
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
        // glGenerateMipmap(GL_TEXTURE_2D);

        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TextureBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (m_TextureBuffer)
            stbi_image_free(m_TextureBuffer);
    }

    Texture::Texture(uint8_t* bytes, uint32_t size): m_RenderID(0), m_Filepath(""), m_TextureBuffer(bytes), m_BPP(size)
    {
        glGenTextures(1, &m_RenderID);
        glBindTexture(GL_TEXTURE_2D, m_RenderID);  

        uint32_t dataBuffer;
        glGenBuffers(1, &dataBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
        glBufferData(GL_ARRAY_BUFFER, size, bytes, GL_STATIC_READ);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
        
        // glTexBuffer(GL_TEXTURE_2D, GL_RGB32I, dataBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_RenderID);
    }

    void Texture::Bind() const 
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_RenderID);
    }

    void Texture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};