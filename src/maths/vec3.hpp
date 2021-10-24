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
    class vec3
    {
        private:
            IntegerType m_X;
            IntegerType m_Y;
            IntegerType m_Z;

        public:
            vec3(IntegerType x = 0, IntegerType y = 0, IntegerType z = 0): m_X(x), m_Y(y), m_Z(z){};
            ~vec3(){}

            // Getters and setters.
            inline IntegerType GetX() const{ return m_X; }
            inline IntegerType GetY() const{ return m_Y; }
            inline IntegerType GetZ() const{ return m_Z; }

            inline void SetX(IntegerType value) { m_X = value; }
            inline void SetY(IntegerType value) { m_Y = value; }
            inline void SetZ(IntegerType value) { m_Z = value; }



            // Math operations
            inline vec3<IntegerType> Add        (const vec3<IntegerType>& other)  const
            { return vec3<IntegerType>(GetX() + other.GetX(), this->GetY() + other.GetY(), this->GetZ() + other.GetZ()); }
            inline vec3<IntegerType> Substract  (const vec3<IntegerType>& other)  const
            { return vec3<IntegerType>(GetX() - other.GetX(), this->GetY() - other.GetY(), this->GetZ() - other.GetZ()); }
            inline vec3<IntegerType> Multiply   (const vec3<IntegerType>& other)  const
            { return vec3<IntegerType>(GetX() * other.GetX(), this->GetY() * other.GetY(), this->GetZ() * other.GetZ()); }
            inline vec3<IntegerType> Divide     (const vec3<IntegerType>& other)  const
            { return vec3<IntegerType>(GetX() / other.GetX(), this->GetY() / other.GetY(), this->GetZ() / other.GetZ()); }

            inline vec3<IntegerType>& SetEqualTo (const vec3<IntegerType>& other)      { *this = other; return *this; }

            inline vec3<IntegerType>& operator= (const vec3<IntegerType>& other)       { return SetEqualTo(other); }

            inline vec3<IntegerType>  operator+  (const vec3<IntegerType>& other) const{ return Add(other); }
            inline vec3<IntegerType>& operator+= (const vec3<IntegerType>& other)      { return SetEqualTo(Add(other)); }
            inline vec3<IntegerType>& operator++ ()                                    { return SetEqualTo(Add(vec3<IntegerType>((IntegerType)1, (IntegerType)1, (IntegerType)1))); }

            inline vec3<IntegerType>  operator-  (const vec3<IntegerType>& other) const{ return Substract(other); }
            inline vec3<IntegerType>& operator-= (const vec3<IntegerType>& other)      { return SetEqualTo(Substract(other)); }
            inline vec3<IntegerType>& operator--  ()                                   { return SetEqualTo(Substract(vec3<IntegerType>((IntegerType)1, (IntegerType)1, (IntegerType)1))); }

            inline vec3<IntegerType>  operator*  (const vec3<IntegerType>& other) const{ return Multiply(other); }
            inline vec3<IntegerType>& operator*= (const vec3<IntegerType>& other)      { return SetEqualTo(Multiply(other)); }

            inline vec3<IntegerType>  operator/  (const vec3<IntegerType>& other) const{ return Divide(other); }
            inline vec3<IntegerType>& operator/= (const vec3<IntegerType>& other)      { return SetEqualTo(Divide(other)); }


            // Logical operations
            inline bool IsEqualTo         (const vec3<IntegerType>& other) const
            { return ((this->GetX() == other.GetX()) && (this->GetY() == other.m_Y) && (this->GetZ() == other.GetZ())); }
            inline bool IsNotEqualTo      (const vec3<IntegerType>& other) const
            { return (!this->IsEqualTo(other)); }

            inline bool IsGreaterThan     (const vec3<IntegerType>& other) const
            { return ((this->GetX() > other.GetX()) && (this->GetY() > other.m_Y) && (this->GetZ() > other.GetZ())); }
            inline bool IsGreaterOrEqualTo(const vec3<IntegerType>& other) const
            { return ((this->GetX() >= other.GetX()) && (this->GetY() >= other.m_Y) && (this->GetZ() >= other.GetZ())); }
            
            inline bool IsLessThan        (const vec3<IntegerType>& other) const
            { return ((this->GetX() < other.GetX()) && (this->GetY() < other.m_Y) && (this->GetZ() < other.GetZ())); }
            inline bool IsLessOrEqualTo   (const vec3<IntegerType>& other) const
            { return ((this->GetX() <= other.GetX()) && (this->GetY() <= other.m_Y) && (this->GetZ() <= other.GetZ())); }
            

            
            inline bool operator==(const vec3<IntegerType>& other) const{ return (this->IsEqualTo(other)); }
            inline bool operator!=(const vec3<IntegerType>& other) const{ return (this->IsNotEqualTo(other)); }
            
            inline bool operator> (const vec3<IntegerType>& other) const{ return (this->IsGreaterThan(other)); }
            inline bool operator>=(const vec3<IntegerType>& other) const{ return (this->IsGreaterOrEqualTo(other)); }

            inline bool operator< (const vec3<IntegerType>& other) const{ return (this->IsLessThan(other)); }
            inline bool operator<=(const vec3<IntegerType>& other) const{ return (this->IsLessOrEqualTo(other)); }

            inline IntegerType Magnitude() const{ return sqrt(GetX() * GetX() + GetY() * GetY() + GetZ() * GetZ()); }
            inline vec3<IntegerType> Normalize() const{ return vec3 (GetX() / Magnitude(), GetY() / Magnitude(), GetZ() / Magnitude()); }
            inline IntegerType Distance(const vec3<IntegerType>& other) const{ return sqrt(pow(GetX() - other.GetX(), 2) + pow(GetY() - other.GetY(), 2) + pow(GetZ() - other.GetZ(), 2)); }

            //inline friend std::ostream& operator<<(std::ostream& stream, const vec3<IntegerType>& vector) { return stream << "vec3: (" << vector.GetX() << ", " << vector.GetY() << ", " << vector.GetZ() << ")."; }
    };
};