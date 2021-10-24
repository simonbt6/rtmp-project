#pragma once

/**
 * @author Simon Brisebois-Therrien 
 * @date 2021-10-14
 * 
 */

#include <maths/vec4.hpp>

namespace Graphics
{
    class Color : public Maths::vec4<float>
    {
        private:
            using Maths::vec4<float>::GetX;
            using Maths::vec4<float>::GetY;
            using Maths::vec4<float>::GetZ;
            using Maths::vec4<float>::GetW;

            using Maths::vec4<float>::SetX;
            using Maths::vec4<float>::SetY;
            using Maths::vec4<float>::SetZ;
            using Maths::vec4<float>::SetW;


        public:
            Color(float r, float g, float b, float a): Maths::vec4<float>(r,g,b, a) {};

            inline float GetR() const { return m_X; }
            inline float GetG() const { return m_Y; }
            inline float GetB() const { return m_Z; }
            inline float GetA() const { return m_W; }

            inline void SetR(float value) { SetX(value); }
            inline void SetG(float value) { SetY(value); }
            inline void SetB(float value) { SetZ(value); }
            inline void SetA(float value) { SetW(value); }

    };
};