#pragma once
/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-14 
 * 
 * 
 */

#include <graphics/IRenderable2D.hpp>
#include <graphics/color.hpp>

#include <maths/maths.hpp>


namespace Graphics
{
    namespace Primitives
    {
        class Sprite: public IRenderable2D
        {

            private:

            public:

                Sprite(Texture* texture, const Color& color, const Maths::Rectangle bounds)
                    : Sprite(texture, bounds.GetPosition(), bounds.GetSize(), color) {}
            
                Sprite(Texture* texture, const Maths::vec2& position, const Maths::vec2& size, const Color& color)
                    :IRenderable2D(position, size, color)
                {
                    SetTexture(texture);
                };

                virtual void Submit(Renderer2D* renderer) const 
                {
                    renderer->DrawSprite(GetTexture(), GetBounds());
                }
        };
    };
};