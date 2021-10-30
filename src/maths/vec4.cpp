#include "vec4.hpp"

#include "mat4.hpp"

namespace Maths
{

    vec4::vec4(float value)
        : vec4(value, value, value, value)
    {
        
    }

    vec4::vec4(float x = 0, float y = 0, float z = 0, float w = 0)
        : m_X(x), m_Y(y), m_Z(z), m_W(w)
    {
        
    }

    vec4::vec4(vec3 xyz, float w)
        : m_X(xyz.GetX()), m_Y(xyz.GetY()), m_Z(xyz.GetZ()), m_W(w)
    {

    }

    vec4::~vec4()
    {

    }

    float vec4::GetX() const
    {
        return m_X;
    }

    float vec4::GetY() const
    {
        return m_Y;
    }

    float vec4::GetZ() const
    {
        return m_Z;
    }

    float vec4::GetW() const
    {
        return m_W;
    }

    void vec4::SetX(float value)
    { 
        m_X = value;
    }

    void vec4::SetY(float value)
    { 
        m_Y = value;
    }

    void vec4::SetZ(float value)
    { 
        m_Z = value;
    }

    void vec4::SetW(float value)
    { 
        m_W = value;
    }

    vec4 vec4::Add(const vec4& other) const
    { 
        return vec4(
            GetX() + other.GetX(), 
            this->GetY() + other.GetY(), 
            this->GetZ() + other.GetZ(), 
            this->GetW() + other.GetW()
        ); 
    }

    vec4 vec4::Substract(const vec4& other) const
    { 
        return vec4(
            GetX() - other.GetX(), 
            this->GetY() - other.GetY(), 
            this->GetZ() - other.GetZ(), 
            this->GetW() - other.GetW()
        ); 
    }

    vec4 vec4::Multiply(const vec4& other) const
    { 
        return vec4(
            GetX() * other.GetX(), 
            this->GetY() * other.GetY(), 
            this->GetZ() * other.GetZ(), 
            this->GetW() * other.GetW()
        ); 
    }

    vec4 vec4::Divide(const vec4& other) const
    { 
        return vec4(
            GetX() / other.GetX(), 
            this->GetY() / other.GetY(), 
            this->GetZ() / other.GetZ(), 
            this->GetW() / other.GetW()
        ); 
    }

    vec4& vec4::SetEqualTo(const vec4& other)
    { 
        *this = other;
        return *this;
    }

    vec4& vec4::operator=(const vec4& other)
    { 
        return SetEqualTo(other); 
    }

    vec4 vec4::operator+(const vec4& other) const
    { 
        return Add(other); 
    }

    vec4& vec4::operator+=(const vec4& other)      
    { 
        return SetEqualTo(Add(other));
    }

    vec4& vec4::operator++()
    { 
        return SetEqualTo(
                Add(
                    vec4(
                        (float)1, 
                        (float)1, 
                        (float)1, 
                        (float)1
                    )));
    }

    vec4  vec4::operator-(const vec4& other) const
    { 
        return Substract(other);
    }

    vec4& vec4::operator-=(const vec4& other)      
    { 
        return SetEqualTo(Substract(other));
    }

    vec4& vec4::operator--()
    { 
        return SetEqualTo(
            Substract(
                vec4(
                    (float)1, 
                    (float)1, 
                    (float)1, 
                    (float)1
                )));
    }

    vec4 vec4::operator*(const vec4& other) const
    { 
        return Multiply(other);
    }

    vec4& vec4::operator*=(const vec4& other)
    { 
        return SetEqualTo(
            Multiply(other)
        );
    }

    vec4  vec4::operator/(const vec4& other) const
    { 
        return Divide(other); 
    }

    vec4& vec4::operator/=(const vec4& other)
    { 
        return SetEqualTo(
            Divide(other)
        ); 
    }

    vec4 vec4::Multiply(const mat4& matrix) const
    {
        return vec4(
            matrix.GetRow(0).GetX() * m_X + matrix.GetRow(0).GetY() * m_Y + matrix.GetRow(0).GetZ() * m_Z + matrix.GetRow(0).GetW() * m_W,
            matrix.GetRow(1).GetX() * m_X + matrix.GetRow(1).GetY() * m_Y + matrix.GetRow(1).GetZ() * m_Z + matrix.GetRow(1).GetW() * m_W,
            matrix.GetRow(2).GetX() * m_X + matrix.GetRow(2).GetY() * m_Y + matrix.GetRow(2).GetZ() * m_Z + matrix.GetRow(2).GetW() * m_W,
            matrix.GetRow(3).GetX() * m_X + matrix.GetRow(3).GetY() * m_Y + matrix.GetRow(3).GetZ() * m_Z + matrix.GetRow(3).GetW() * m_W
        );
    }

    bool vec4::IsEqualTo(const vec4& other) const
    { 
        return (
               (this->GetX() == other.GetX())
            && (this->GetY() == other.m_Y) 
            && (this->GetZ() == other.GetZ()) 
            && (this->GetW() == other.GetW())
        ); 
    }

    bool vec4::IsNotEqualTo(const vec4& other) const
    { 
        return (!IsEqualTo(other)); 
    }

    bool vec4::IsGreaterThan(const vec4& other) const
    { 
        return (
               (this->GetX() > other.GetX()) 
            && (this->GetY() > other.GetY()) 
            && (this->GetZ() > other.GetZ()) 
            && (this->GetW() > other.GetW())
        ); 
    }

    bool vec4::IsGreaterOrEqualTo(const vec4& other) const
    { 
        return (
               (this->GetX() >= other.GetX()) 
            && (this->GetY() >= other.m_Y) 
            && (this->GetZ() >= other.GetZ()) 
            && (this->GetW() >= other.GetW())
        ); 
    }

    bool vec4::IsLessThan(const vec4& other) const
    { 
        return (
               (this->GetX() < other.GetX()) 
            && (this->GetY() < other.m_Y) 
            && (this->GetZ() < other.GetZ()) 
            && (this->GetW() < other.GetW())
        );
    }

    bool vec4::IsLessOrEqualTo(const vec4& other) const
    { 
        return (
               (this->GetX() <= other.GetX()) 
            && (this->GetY() <= other.m_Y) 
            && (this->GetZ() <= other.GetZ())
            && (this->GetW() <= other.GetW())
        ); 
    }

    bool vec4::operator==(const vec4& other) const
    { 
        return (this->IsEqualTo(other)); 
    }

    bool vec4::operator!=(const vec4& other) const
    { 
        return (this->IsNotEqualTo(other)); 
    }

    bool vec4::operator>(const vec4& other) const
    { 
        return (this->IsGreaterThan(other)); 
    }

    bool vec4::operator>=(const vec4& other) const
    { 
        return (this->IsGreaterOrEqualTo(other)); 
    }

    bool vec4::operator<(const vec4& other) const
    { 
        return (this->IsLessThan(other)); 
    }

    bool vec4::operator<=(const vec4& other) const
    { 
        return (this->IsLessOrEqualTo(other)); 
    }
    
    
    



    
















    

    



    
};