#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-13 
 * 
 * 
 */

namespace Maths
{
    template <typename IntegerType>
    class vec4
    {
        protected:
            IntegerType m_X;
            IntegerType m_Y;
            IntegerType m_Z;
            IntegerType m_W;

        public:
            vec4(IntegerType x = 0, IntegerType y = 0, IntegerType z = 0, IntegerType w = 0): m_X(x), m_Y(y), m_Z(z), m_W(w){};
            ~vec4(){}

            // Getters and setters.
            inline IntegerType GetX() const{ return m_X; }
            inline IntegerType GetY() const{ return m_Y; }
            inline IntegerType GetZ() const{ return m_Z; }
            inline IntegerType GetW() const{ return m_W; }

            inline void SetX(IntegerType value) { m_X = value; }
            inline void SetY(IntegerType value) { m_Y = value; }
            inline void SetZ(IntegerType value) { m_Z = value; }
            inline void SetW(IntegerType value) { m_W = value; }


            // Math operations
            inline vec4<IntegerType> Add        (const vec4<IntegerType>& other)  const
            { return vec4<IntegerType>(GetX() + other.GetX(), this->GetY() + other.GetY(), this->GetZ() + other.GetZ(), this->GetW() + other.GetW()); }
            inline vec4<IntegerType> Substract  (const vec4<IntegerType>& other)  const
            { return vec4<IntegerType>(GetX() - other.GetX(), this->GetY() - other.GetY(), this->GetZ() - other.GetZ(), this->GetW() - other.GetW()); }
            inline vec4<IntegerType> Multiply   (const vec4<IntegerType>& other)  const
            { return vec4<IntegerType>(GetX() * other.GetX(), this->GetY() * other.GetY(), this->GetZ() * other.GetZ(), this->GetW() * other.GetW()); }
            inline vec4<IntegerType> Divide     (const vec4<IntegerType>& other)  const
            { return vec4<IntegerType>(GetX() / other.GetX(), this->GetY() / other.GetY(), this->GetZ() / other.GetZ(), this->GetW() / other.GetW()); }


            inline vec4<IntegerType>& SetEqualTo (const vec4<IntegerType>& other)      { *this = other; return *this; }

            inline vec4<IntegerType>& operator= (const vec4<IntegerType>& other)       { return SetEqualTo(other); }

            inline vec4<IntegerType>  operator+  (const vec4<IntegerType>& other) const{ return Add(other); }
            inline vec4<IntegerType>& operator+= (const vec4<IntegerType>& other)      { return SetEqualTo(Add(other)); }
            inline vec4<IntegerType>& operator++ ()                                    { return SetEqualTo(Add(vec4<IntegerType>((IntegerType)1, (IntegerType)1, (IntegerType)1, (IntegerType)1))); }

            inline vec4<IntegerType>  operator-  (const vec4<IntegerType>& other) const{ return Substract(other); }
            inline vec4<IntegerType>& operator-= (const vec4<IntegerType>& other)      { return SetEqualTo(Substract(other)); }
            inline vec4<IntegerType>& operator--  ()                                   { return SetEqualTo(Substract(vec4<IntegerType>((IntegerType)1, (IntegerType)1, (IntegerType)1, (IntegerType)1))); }

            inline vec4<IntegerType>  operator*  (const vec4<IntegerType>& other) const{ return Multiply(other); }
            inline vec4<IntegerType>& operator*= (const vec4<IntegerType>& other)      { return SetEqualTo(Multiply(other)); }

            inline vec4<IntegerType>  operator/  (const vec4<IntegerType>& other) const{ return Divide(other); }
            inline vec4<IntegerType>& operator/= (const vec4<IntegerType>& other)      { return SetEqualTo(Divide(other)); }


            // Logical operations
            inline bool IsEqualTo         (const vec4<IntegerType>& other) const
            { return ((this->GetX() == other.GetX()) && (this->GetY() == other.m_Y) && (this->GetZ() == other.GetZ()) && (this->GetW() == other.GetW())); }
            inline bool IsNotEqualTo      (const vec4<IntegerType>& other) const
            { return (!IsEqualTo(other)); }

            inline bool IsGreaterThan     (const vec4<IntegerType>& other) const
            { return ((this->GetX() > other.GetX()) && (this->GetY() > other.m_Y) && (this->GetZ() > other.GetZ()) && (this->GetW() > other.GetW())); }
            inline bool IsGreaterOrEqualTo(const vec4<IntegerType>& other) const
            { return ((this->GetX() >= other.GetX()) && (this->GetY() >= other.m_Y) && (this->GetZ() >= other.GetZ()) && (this->GetW() >= other.GetW())); }
            
            inline bool IsLessThan        (const vec4<IntegerType>& other) const
            { return ((this->GetX() < other.GetX()) && (this->GetY() < other.m_Y) && (this->GetZ() < other.GetZ()) && (this->GetW() < other.GetW())); }
            inline bool IsLessOrEqualTo   (const vec4<IntegerType>& other) const
            { return ((this->GetX() <= other.GetX()) && (this->GetY() <= other.m_Y) && (this->GetZ() <= other.GetZ()) && (this->GetW() <= other.GetW())); }
            

            
            inline bool operator==(const vec4<IntegerType>& other) const{ return (this->IsEqualTo(other)); }
            inline bool operator!=(const vec4<IntegerType>& other) const{ return (this->IsNotEqualTo(other)); }
            
            inline bool operator> (const vec4<IntegerType>& other) const{ return (this->IsGreaterThan(other)); }
            inline bool operator>=(const vec4<IntegerType>& other) const{ return (this->IsGreaterOrEqualTo(other)); }

            inline bool operator< (const vec4<IntegerType>& other) const{ return (this->IsLessThan(other)); }
            inline bool operator<=(const vec4<IntegerType>& other) const{ return (this->IsLessOrEqualTo(other)); }

    };
};