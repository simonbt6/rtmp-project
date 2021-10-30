#include "Font.hpp"

namespace Graphics
{
    Font::Font(const std::string& name, const std::string& filename, uint32_t size, FT_Library* ft)
        : m_Size(size), m_Filename(filename), m_Name(name)
    {
        m_CharacterTextures.resize(128);

        m_Face = new FT_Face();

        if (FT_New_Face(*ft, filename.c_str(), size, m_Face))
            throw std::runtime_error("\nFailed to create new face.");

        FT_Set_Char_Size(*m_Face, size << 6, size << 6, 96, 96);

        m_ListBase = glGenLists(128);
        glGenTextures(128, &m_CharacterTextures.front());

        for (uint8_t i = 0; i < 128; i++)
            GenDlist(*m_Face, i, m_ListBase, &m_CharacterTextures.front());
        
        FT_Done_Face(*m_Face);

    }

    Font::~Font()
    {
        glDeleteLists(m_ListBase, 128);
        glDeleteTextures(128, &m_CharacterTextures.front());
    }

    void Font::GenDlist(FT_Face face, char character, uint32_t list_base, uint32_t* tex_base)
    {
        FT_BitmapGlyph bitmap_glyph = GenerateBitmapForFace(face, character);

        FT_Bitmap& bitmap = bitmap_glyph->bitmap;

        int width  = next_p2(bitmap.width);
        int height = next_p2(bitmap.rows);

        std::vector<uint8_t> expanded_data(2 * width * height, 0);

        StoreTextureData(width, height, bitmap, expanded_data);

        glBindTexture(GL_TEXTURE_2D, tex_base[character]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, &expanded_data.front());

        glNewList(list_base + character, GL_COMPILE);

        glBindTexture(GL_TEXTURE_2D, tex_base[character]);

        glPushMatrix();

        glTranslatef(bitmap_glyph->left, 0, 0);
        glTranslatef(0, bitmap_glyph->top - (bitmap.rows * 0.9f), 0);

        float x = (float) bitmap.width / (float) width,
              y = (float) bitmap.rows  / (float) height;

        glBegin(GL_QUADS);
            glTexCoord2d(0, 0); glVertex2f(0, bitmap.rows);
            glTexCoord2d(0, y); glVertex2f(0, 0);
            glTexCoord2d(x, y); glVertex2f(bitmap.width, 0);
            glTexCoord2d(x, 0); glVertex2f(bitmap.width, bitmap.rows);
        glEnd();
        glPopMatrix();

        glTranslatef(face->glyph->advance.x >> 6, 0, 0);
        glEndList();
        
    }

    void Font::StoreTextureData(int32_t width, int32_t height, FT_Bitmap& bitmap, std::vector<uint8_t>& expanded_data)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                expanded_data[2 * (x + y * width)] = 255;
                expanded_data[2 * (x + y * width) + 1] = 
                    (x >= bitmap.width || y >= bitmap.rows) ? 0 :
                        bitmap.buffer[x + bitmap.width * y];
            }
        }
    }



    FT_BitmapGlyph Font::GenerateBitmapForFace(FT_Face face, char character)
    {
        if (FT_Load_Glyph(face, FT_Get_Char_Index(face, character), FT_LOAD_DEFAULT))
            throw std::runtime_error("\nFailed to load glyph.");

        FT_Glyph glyph;
        if (FT_Get_Glyph(face->glyph, &glyph))
            throw std::runtime_error("\nFailed to get glyph.");

        FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);

        return (FT_BitmapGlyph) glyph;
    }


    int Font::next_p2 (int a )
    {
        int rval=1;
        
        while(rval<a) rval<<=1;

        return rval;
    }
}