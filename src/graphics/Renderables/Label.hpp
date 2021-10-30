#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-24 
 * 
 * 
 */

#include <graphics/Font.hpp>
#include <graphics/IRenderable2D.hpp>
#include <graphics/color.hpp>

#include <maths/maths.hpp>

#include <string>

namespace Graphics
{
    namespace Primitives
    {
        class Label : public IRenderable2D
        {
            private:
                std::string m_Text;
                Color m_Color;
                std::string m_FontName;

            public:
                Label(const std::string& text, const std::string& font_name, uint32_t size, const Color& color, const Maths::vec2& position)
                    : IRenderable2D(position, Maths::vec2(size), color), m_Text(text), m_Color(color), m_FontName(font_name) 
                {

                }

                void Submit(Renderer2D* renderer) const
                {
                    renderer->DrawText(m_Text, "arial", GetSize().GetY(), m_Color, m_Bounds.GetPosition());

                }

            private:
        };
    }
    
};  