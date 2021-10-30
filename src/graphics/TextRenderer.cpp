#include "TextRenderer.hpp"

namespace Graphics
{
    TextRenderer::TextRenderer()
    {
        m_FreeType = new FT_Library();
        if (FT_Init_FreeType(m_FreeType)) throw std::runtime_error("\nFailed to init freetype library.");
    }

    TextRenderer::~TextRenderer()
    {

    }

    void TextRenderer::DrawString(const std::string& text, const std::string& font_name, const Color& color, uint32_t size, Maths::vec2 position)
    {
        glColor3ub(color.GetR(), color.GetG(), color.GetB());
        glPushAttrib(GL_TRANSFORM_BIT);
        int32_t viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(viewport[0], viewport[1], viewport[2], viewport[3], -1, 1);
        glPopAttrib();

        Font* font = GetFont(font_name, std::string("fonts/") + font_name + ".ttf", size);

        std::stringstream ss(text);
        std::string line;
        std::vector<std::string> lines;
        while (std::getline(ss, line, '\n'))
            lines.push_back(line);

        glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glListBase(font->GetListBase());

        float modelview_matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

        for (int iLine = 0; iLine < lines.size(); iLine++)
        {
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(position.GetX(), position.GetY() - (font->GetSize() / .63) * iLine, 0);
            glMultMatrixf(modelview_matrix);

            glCallLists(lines[iLine].length(), GL_UNSIGNED_BYTE, lines[iLine].c_str());

            glPopMatrix();
        }

        glPopAttrib();
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glPopAttrib();

    }


    Font* TextRenderer::LoadFont(const std::string& name , const std::string& filename, uint32_t size)
    {
        Font* font = new Font(name, filename, size, m_FreeType);
        if (font == nullptr) throw std::runtime_error("\nFailed to load Font object.");

        if (m_Fonts.find(name) != m_Fonts.end())
            m_Fonts.at(name).insert(std::pair<uint32_t, Font*>(size, font));
        else
            m_Fonts.insert({name, { {size, font} }});
        return font;
    }

    Font* TextRenderer::GetFont(const std::string& name, const std::string& filename, uint32_t size)
    {
        if (m_Fonts.find(name) != m_Fonts.end())
            if (m_Fonts.at(name).find(size) != m_Fonts.at(name).end())
                return m_Fonts.at(name).at(size);
        return LoadFont(name, filename, size);
    }

};