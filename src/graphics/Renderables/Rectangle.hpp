#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-14 
 * 
 * 
 */

#include <glad/gl.h>

#include "../IRenderable2D.hpp"

namespace Graphics
{
    namespace Primitives
    {
        class Rectangle : public IRenderable2D
        {
            private:
                typedef IRenderable2D super;
            
            public:
                ~Rectangle()
                {
                    
                }
        };
    };
};