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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TextureBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (m_TextureBuffer)
            stbi_image_free(m_TextureBuffer);

    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_RenderID);
    }

    void Texture::Bind() const 
    {
        glBindTexture(GL_TEXTURE_2D, m_RenderID);
    }

    void Texture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};