#pragma once


#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include FT_FREETYPE_H

#include "Texture.hpp"
#include "color.hpp"

#include <maths/maths.hpp>

#include <string>

namespace Graphics
{
    struct TextCharacter
    {
        uint32_t TextureID;

        Maths::vec2 Size;
        Maths::vec2 Bearing;

        uint32_t Advance;
    };
    class Font
    {
        private:
            FT_Face* m_Face;

            std::string m_Filename;
            std::string m_Name;

            uint32_t m_Size;
            uint32_t m_ListBase;

            std::map<char, TextCharacter> m_Characters;
            std::vector<uint32_t>         m_CharacterTextures;

        public:
            Font(const std::string& name, const std::string& filename, uint32_t size, FT_Library* ft);
            ~Font();

            uint32_t GenTextureFromString(const std::string& text, const Color& color);

            inline uint32_t GetSize() const { return m_Size; }
            inline const std::string& GetName() const { return m_Name; }
            inline const std::string& GetFilename() const { return m_Filename; }

            inline uint32_t GetListBase() const { return m_ListBase; }
            
        private:
            void GenDlist(FT_Face face, char character, uint32_t list_base, uint32_t* tex_base);
            void StoreTextureData(int32_t width, int32_t height, FT_Bitmap& bitmap, std::vector<uint8_t>& expanded_data);
            FT_BitmapGlyph GenerateBitmapForFace(FT_Face face, char character);

            int next_p2 (int a );
            
    };
};