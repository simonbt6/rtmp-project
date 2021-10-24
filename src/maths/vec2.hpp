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
    class vec2
    {
        private:
            IntegerType m_X;
            IntegerType m_Y;

        public:
            vec2(IntegerType x = 0, IntegerType y = 0): m_X(x), m_Y(y){};
            ~vec2(){}

            // Getters and setters.
            inline IntegerType GetX() const{ return m_X; }
            inline IntegerType GetY() const{ return m_Y; }

            inline IntegerType* AsArray() const{ return new IntegerType[2]{ m_X, m_Y }; }

            inline void SetX(IntegerType value) { m_X = value; }
            inline void SetY(IntegerType value) { m_Y = value; }



            // Math operations

            inline vec2<IntegerType> Add        (const vec2<IntegerType>& other)  const{ return vec2<IntegerType>(GetX() + other.GetX(), this->GetY() + other.GetY()); }
            inline vec2<IntegerType> Substract  (const vec2<IntegerType>& other)  const{ return vec2<IntegerType>(GetX() - other.GetX(), this->GetY() - other.GetY()); }
            inline vec2<IntegerType> Multiply   (const vec2<IntegerType>& other)  const{ return vec2<IntegerType>(GetX() * other.GetX(), this->GetY() * other.GetY()); }
            inline vec2<IntegerType> Divide     (const vec2<IntegerType>& other)  const{ return vec2<IntegerType>(GetX() / other.GetX(), this->GetY() / other.GetY()); }

            inline vec2<IntegerType>& SetEqualTo (const vec2<IntegerType>& other)      { SetX(other.GetX()); SetY(other.GetY()); return *this; }

            inline vec2<IntegerType>& operator= (const vec2<IntegerType>& other)       { return SetEqualTo(other); }

            inline vec2<IntegerType>  operator+  (const vec2<IntegerType>& other) const{ return Add(other); }
            inline vec2<IntegerType>& operator+= (const vec2<IntegerType>& other)      { return SetEqualTo(Add(other)); }
            inline vec2<IntegerType>& operator++ ()                                    { return SetEqualTo(Add(vec2<IntegerType>(1, 1))); }

            inline vec2<IntegerType>  operator-  (const vec2<IntegerType>& other) const{ return Substract(other); }
            inline vec2<IntegerType>& operator-= (const vec2<IntegerType>& other)      { return SetEqualTo(Substract(other)); }
            inline vec2<IntegerType>& operator--  ()                                   { return SetEqualTo(Substract(vec2<IntegerType>(1, 1))); }

            inline vec2<IntegerType>  operator*  (const vec2<IntegerType>& other) const{ return Multiply(other); }
            inline vec2<IntegerType>& operator*= (const vec2<IntegerType>& other)      { return SetEqualTo(Multiply(other)); }

            inline vec2<IntegerType>  operator/  (const vec2<IntegerType>& other) const{ return Divide(other); }
            inline vec2<IntegerType>& operator/= (const vec2<IntegerType>& other)      { return SetEqualTo(Divide(other)); }


            // Logical operations
            inline bool IsEqualTo         (const vec2<IntegerType>& other) const{ return ((this->GetX() == other.GetX()) && (this->GetY() == other.m_Y)); }
            inline bool IsNotEqualTo      (const vec2<IntegerType>& other) const{ return (!this->IsEqualTo(other)); }

            inline bool IsGreaterThan     (const vec2<IntegerType>& other) const{ return ((this->GetX() > other.GetX()) && (this->GetY() > other.m_Y)); }
            inline bool IsGreaterOrEqualTo(const vec2<IntegerType>& other) const{ return ((this->GetX() >= other.GetX()) && (this->GetY() >= other.m_Y)); }
            
            inline bool IsLessThan        (const vec2<IntegerType>& other) const{ return ((this->GetX() < other.GetX()) && (this->GetY() < other.m_Y)); }
            inline bool IsLessOrEqualTo   (const vec2<IntegerType>& other) const{ return ((this->GetX() <= other.GetX()) && (this->GetY() <= other.m_Y)); }
            

            
            inline bool operator==(const vec2<IntegerType>& other) const{ return (this->IsEqualTo(other)); }
            inline bool operator!=(const vec2<IntegerType>& other) const{ return (this->IsNotEqualTo(other)); }
            
            inline bool operator> (const vec2<IntegerType>& other) const{ return (this->IsGreaterThan(other)); }
            inline bool operator>=(const vec2<IntegerType>& other) const{ return (this->IsGreaterOrEqualTo(other)); }

            inline bool operator< (const vec2<IntegerType>& other) const{ return (this->IsLessThan(other)); }
            inline bool operator<=(const vec2<IntegerType>& other) const{ return (this->IsLessOrEqualTo(other)); }

    };
};