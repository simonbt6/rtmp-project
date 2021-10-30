#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-14 
 * 
 * 
 */

#include "vec2.hpp"


namespace Maths
{
    class Rectangle
    {
        private:
            vec2 m_Position;
            vec2 m_Size;

        public:

            Rectangle(): m_Position(vec2(0, 0)), m_Size(vec2(0, 0)) {}
            Rectangle(const vec2& position, const vec2& size): m_Position(position), m_Size(size) {}
            Rectangle(float x, float y, float width, float height): m_Position(vec2(x, y)), m_Size(vec2(width, height)){}

            ~Rectangle(){}

            inline vec2 GetMinBound() const { return m_Position - m_Size; }
            inline vec2 GetMaxBound() const { return m_Position + m_Size; }

            
            inline const vec2& GetPosition() const{ return m_Position; }
            inline void SetPosition(const vec2& position) { m_Position = position; }

            inline const vec2& GetSize() const { return m_Size; }
            inline void SetSize(const vec2& size) { m_Size = size; }

    };
};