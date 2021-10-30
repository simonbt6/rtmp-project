#pragma once

/**
 * @author Simon Brisebois-Therrien 
 * @date 2021-10-14
 * 
 */

#include <maths/maths.hpp>

namespace Graphics
{
    class Color : public Maths::vec4
    {
        private:
            using Maths::vec4::GetX;
            using Maths::vec4::GetY;
            using Maths::vec4::GetZ;
            using Maths::vec4::GetW;

            using Maths::vec4::SetX;
            using Maths::vec4::SetY;
            using Maths::vec4::SetZ;
            using Maths::vec4::SetW;


        public:
            Color(float r, float g, float b, float a): Maths::vec4(r,g,b, a) {};

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