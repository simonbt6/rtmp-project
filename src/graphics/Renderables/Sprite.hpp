#pragma once
/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-14 
 * 
 * 
 */

#include <IRenderable2D.hpp>
#include <utils/vec2.hpp>
#include <utils/vec3.hpp>
#include <utils/vec4.hpp>
#include <utils/color.hpp>
#include <utils/rectangle.hpp>

namespace Graphics
{
    namespace Primitives
    {
        class Sprite: public IRenderable2D
        {
            public:
            
                Sprite(const Maths::vec2<float>& position, const Maths::vec2<float>& size, const Maths::Color& color)
                :IRenderable2D(position, size, color)
                {
                    const Maths::vec2<float>& p = m_Bounds.GetPosition();
                    const Maths::vec2<float>& s = m_Bounds.GetSize();

                    const float width  = s.GetX(), 
                                height = s.GetY(),
                                x      = p.GetX(),
                                y      = p.GetY();

                    SetTexture(new Texture("Red.png"));
                    Maths::vec2(x, y);
                    m_UV.insert(m_UV.end(), {
                        Maths::vec4((-width + x), (-height + y), 0.0f, 0.0f),
                        Maths::vec4(( width + x), (-height + y), 1.0f, 0.0f),
                        Maths::vec4(( width + x), ( height + y), 1.0f, 1.0f),
                        Maths::vec4((-width + x), ( height + y), 0.0f, 1.0f) 
                    });
                    m_Indices.insert(m_Indices.end(), {
                        0, 1, 2,    2, 3, 0
                    });
                };
        };
    };
};