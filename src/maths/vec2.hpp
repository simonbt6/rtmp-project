#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-13 
 * 
 * 
 */

namespace Maths
{
    class vec2
    {
        private:
            float m_X;
            float m_Y;

        public:
            vec2(float x = 0, float y = 0): m_X(x), m_Y(y){};
            ~vec2(){}

            // Getters and setters.
            inline float GetX() const{ return m_X; }
            inline float GetY() const{ return m_Y; }

            inline float* AsArray() const{ return new float[2]{ m_X, m_Y }; }

            inline void SetX(float value) { m_X = value; }
            inline void SetY(float value) { m_Y = value; }



            // Math operations

            inline vec2 Add        (const vec2& other)  const{ return vec2(GetX() + other.GetX(), this->GetY() + other.GetY()); }
            inline vec2 Substract  (const vec2& other)  const{ return vec2(GetX() - other.GetX(), this->GetY() - other.GetY()); }
            inline vec2 Multiply   (const vec2& other)  const{ return vec2(GetX() * other.GetX(), this->GetY() * other.GetY()); }
            inline vec2 Divide     (const vec2& other)  const{ return vec2(GetX() / other.GetX(), this->GetY() / other.GetY()); }

            inline vec2& SetEqualTo (const vec2& other)      { SetX(other.GetX()); SetY(other.GetY()); return *this; }

            inline vec2& operator= (const vec2& other)       { return SetEqualTo(other); }

            inline vec2  operator+  (const vec2& other) const{ return Add(other); }
            inline vec2& operator+= (const vec2& other)      { return SetEqualTo(Add(other)); }
            inline vec2& operator++ ()                                    { return SetEqualTo(Add(vec2(1, 1))); }

            inline vec2  operator-  (const vec2& other) const{ return Substract(other); }
            inline vec2& operator-= (const vec2& other)      { return SetEqualTo(Substract(other)); }
            inline vec2& operator--  ()                                   { return SetEqualTo(Substract(vec2(1, 1))); }

            inline vec2  operator*  (const vec2& other) const{ return Multiply(other); }
            inline vec2& operator*= (const vec2& other)      { return SetEqualTo(Multiply(other)); }

            inline vec2  operator/  (const vec2& other) const{ return Divide(other); }
            inline vec2& operator/= (const vec2& other)      { return SetEqualTo(Divide(other)); }


            // Logical operations
            inline bool IsEqualTo         (const vec2& other) const{ return ((this->GetX() == other.GetX()) && (this->GetY() == other.m_Y)); }
            inline bool IsNotEqualTo      (const vec2& other) const{ return (!this->IsEqualTo(other)); }

            inline bool IsGreaterThan     (const vec2& other) const{ return ((this->GetX() > other.GetX()) && (this->GetY() > other.m_Y)); }
            inline bool IsGreaterOrEqualTo(const vec2& other) const{ return ((this->GetX() >= other.GetX()) && (this->GetY() >= other.m_Y)); }
            
            inline bool IsLessThan        (const vec2& other) const{ return ((this->GetX() < other.GetX()) && (this->GetY() < other.m_Y)); }
            inline bool IsLessOrEqualTo   (const vec2& other) const{ return ((this->GetX() <= other.GetX()) && (this->GetY() <= other.m_Y)); }
            

            
            inline bool operator==(const vec2& other) const{ return (this->IsEqualTo(other)); }
            inline bool operator!=(const vec2& other) const{ return (this->IsNotEqualTo(other)); }
            
            inline bool operator> (const vec2& other) const{ return (this->IsGreaterThan(other)); }
            inline bool operator>=(const vec2& other) const{ return (this->IsGreaterOrEqualTo(other)); }

            inline bool operator< (const vec2& other) const{ return (this->IsLessThan(other)); }
            inline bool operator<=(const vec2& other) const{ return (this->IsLessOrEqualTo(other)); }

    };
};