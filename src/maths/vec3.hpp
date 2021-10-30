#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-13 
 * 
 * 
 */

#include <math.h>

namespace Maths
{
    class vec2;
    class vec4;
    class mat4;

    class vec3
    {
        private:
            float m_X;
            float m_Y;
            float m_Z;

        public:
            vec3(float value): vec3(value, value, value){};
            vec3(float x = 0, float y = 0, float z = 0): m_X(x), m_Y(y), m_Z(z){};
            ~vec3(){}

            // Getters and setters.
            inline float GetX() const{ return m_X; }
            inline float GetY() const{ return m_Y; }
            inline float GetZ() const{ return m_Z; }

            inline void SetX(float value) { m_X = value; }
            inline void SetY(float value) { m_Y = value; }
            inline void SetZ(float value) { m_Z = value; }



            // Math operations
            inline vec3 Add        (const vec3& other)  const
            { return vec3(GetX() + other.GetX(), this->GetY() + other.GetY(), this->GetZ() + other.GetZ()); }
            inline vec3 Substract  (const vec3& other)  const
            { return vec3(GetX() - other.GetX(), this->GetY() - other.GetY(), this->GetZ() - other.GetZ()); }
            inline vec3 Multiply   (const vec3& other)  const
            { return vec3(GetX() * other.GetX(), this->GetY() * other.GetY(), this->GetZ() * other.GetZ()); }
            inline vec3 Divide     (const vec3& other)  const
            { return vec3(GetX() / other.GetX(), this->GetY() / other.GetY(), this->GetZ() / other.GetZ()); }

            inline vec3 Multiply(const mat4& matrix) const { return *this; };

            inline vec3& SetEqualTo (const vec3& other)      { *this = other; return *this; }

            inline vec3& operator= (const vec3& other)       { return SetEqualTo(other); }

            inline vec3  operator+  (const vec3& other) const{ return Add(other); }
            inline vec3& operator+= (const vec3& other)      { return SetEqualTo(Add(other)); }
            inline vec3& operator++ ()                                    { return SetEqualTo(Add(vec3((float)1, (float)1, (float)1))); }

            inline vec3  operator-  (const vec3& other) const{ return Substract(other); }
            inline vec3& operator-= (const vec3& other)      { return SetEqualTo(Substract(other)); }
            inline vec3& operator--  ()                                   { return SetEqualTo(Substract(vec3((float)1, (float)1, (float)1))); }

            inline vec3  operator*  (const vec3& other) const{ return Multiply(other); }
            inline vec3& operator*= (const vec3& other)      { return SetEqualTo(Multiply(other)); }

            inline vec3  operator/  (const vec3& other) const{ return Divide(other); }
            inline vec3& operator/= (const vec3& other)      { return SetEqualTo(Divide(other)); }


            // Logical operations
            inline bool IsEqualTo         (const vec3& other) const
            { return ((this->GetX() == other.GetX()) && (this->GetY() == other.m_Y) && (this->GetZ() == other.GetZ())); }
            inline bool IsNotEqualTo      (const vec3& other) const
            { return (!this->IsEqualTo(other)); }

            inline bool IsGreaterThan     (const vec3& other) const
            { return ((this->GetX() > other.GetX()) && (this->GetY() > other.m_Y) && (this->GetZ() > other.GetZ())); }
            inline bool IsGreaterOrEqualTo(const vec3& other) const
            { return ((this->GetX() >= other.GetX()) && (this->GetY() >= other.m_Y) && (this->GetZ() >= other.GetZ())); }
            
            inline bool IsLessThan        (const vec3& other) const
            { return ((this->GetX() < other.GetX()) && (this->GetY() < other.m_Y) && (this->GetZ() < other.GetZ())); }
            inline bool IsLessOrEqualTo   (const vec3& other) const
            { return ((this->GetX() <= other.GetX()) && (this->GetY() <= other.m_Y) && (this->GetZ() <= other.GetZ())); }
            

            
            inline bool operator==(const vec3& other) const{ return (this->IsEqualTo(other)); }
            inline bool operator!=(const vec3& other) const{ return (this->IsNotEqualTo(other)); }
            
            inline bool operator> (const vec3& other) const{ return (this->IsGreaterThan(other)); }
            inline bool operator>=(const vec3& other) const{ return (this->IsGreaterOrEqualTo(other)); }

            inline bool operator< (const vec3& other) const{ return (this->IsLessThan(other)); }
            inline bool operator<=(const vec3& other) const{ return (this->IsLessOrEqualTo(other)); }

            inline float Magnitude() const{ return sqrt(GetX() * GetX() + GetY() * GetY() + GetZ() * GetZ()); }
            inline vec3 Normalize() const{ return vec3 (GetX() / Magnitude(), GetY() / Magnitude(), GetZ() / Magnitude()); }
            inline float Distance(const vec3& other) const{ return sqrt(pow(GetX() - other.GetX(), 2) + pow(GetY() - other.GetY(), 2) + pow(GetZ() - other.GetZ(), 2)); }

            //inline friend std::ostream& operator<<(std::ostream& stream, const vec3& vector) { return stream << "vec3: (" << vector.GetX() << ", " << vector.GetY() << ", " << vector.GetZ() << ")."; }
    };
};