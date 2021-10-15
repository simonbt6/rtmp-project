#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-11 
 * 
 * 
 */

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <inttypes.h>
#include <string>
#include <vector>

// Utils
#include <FormatedPrint.hpp>
#include <FileManager.hpp>

namespace Graphics
{
    class Texture
    {
        private:
            uint32_t m_RenderID;
            std::string m_Filepath;

            int32_t m_Width, m_Height, m_BPP;
            uint8_t* m_TextureBuffer;

        public:
            Texture(const std::string& path);
            Texture(uint8_t* bytes, uint32_t size);
            ~Texture();


            void Bind() const;
            void Unbind();

            inline int32_t GetWidth() const { return m_Width; }
            inline int32_t GetHeight() const { return m_Height; }
    };
};