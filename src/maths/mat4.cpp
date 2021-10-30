#include "mat4.hpp"

namespace Maths
{
    mat4::mat4()
    {
        memset(m_Elements, 0, 4 * 4 * sizeof(float));
    }

    mat4::mat4(float diagonal)
    {
        memset(m_Elements, 0, 4 * 4 * sizeof(float));
        m_Elements[0 + 0 * 4] = diagonal;
        m_Elements[1 + 1 * 4] = diagonal;
        m_Elements[2 + 2 * 4] = diagonal;
        m_Elements[3 + 3 * 4] = diagonal;
    }

    mat4::mat4(float* elements)
    {
        memcpy(this->m_Elements, elements, 4 * 4 * sizeof(float));   
    }

    
    mat4::mat4(float n0, float n1, float n2, float n3)
    {
        m_Rows[0] = Maths::vec4(n0);
        m_Rows[1] = Maths::vec4(n1);
        m_Rows[2] = Maths::vec4(n2);
        m_Rows[3] = Maths::vec4(n3);
    }

    
    mat4::mat4(const vec4& r0, const vec4& r1, const vec4& r2, const vec4& r3)
    {
        m_Rows[0] = r0;
        m_Rows[1] = r1;   
        m_Rows[2] = r2;   
        m_Rows[3] = r3;   
    }

    mat4::~mat4()
    {

    }

    
    mat4 mat4::Identity()
    {
        return mat4(1.0f);
    }

    mat4& mat4::operator=(const mat4& other)
    {
        this->SetColumn(0, other.GetColumn(0));
        this->SetColumn(1, other.GetColumn(1));
        this->SetColumn(2, other.GetColumn(2));
        this->SetColumn(3, other.GetColumn(3));
        return *this;
    }


    
    mat4& mat4::Multiply(const mat4& other)
    {
        float data[16];
        for (int32_t row = 0; row < 4; row++)
        {
            for (int32_t col = 0; col < 4; col++)
            {
                float sum = (float) 0;
                for (int32_t e = 0; e < 4; e++)
                    sum += m_Elements[e + row * 4] * other.GetElements()[col + e * 4];
                data[col + row * 4] = sum;
                }
        }
        memcpy(m_Elements, data, 4 * 4 * sizeof(float));
        return *this;
    }

    
    mat4 operator*(mat4 left, const mat4& right)
    {
        return left.Multiply(right);
    }

    
    mat4& mat4::operator*=(const mat4& other)
    {
        return Multiply(other);
    }

    
    vec3 mat4::Multiply(const vec3& other) const
    {
        return other.Multiply(*this);
    }

    
    vec3 operator*(const mat4& left, const vec3& right)
    {
        return left.Multiply(right);
    }

    
    vec4 mat4::Multiply(const vec4& other) const
    {
        return other.Multiply(*this);
    }

    
    vec4 operator*(const mat4& left, const vec4& right)
    {
        return left.Multiply(right);
    }

    
    mat4& mat4::Invert()
    {
        float temp[16];

        temp[0]  =  m_Elements[5]  * m_Elements[10] * m_Elements[15] -
                    m_Elements[5]  * m_Elements[11] * m_Elements[14] -
                    m_Elements[9]  * m_Elements[6]  * m_Elements[13] +
                    m_Elements[9]  * m_Elements[7]  * m_Elements[12] +
                    m_Elements[13] * m_Elements[6]  * m_Elements[11] -
                    m_Elements[13] * m_Elements[7]  * m_Elements[10];

        temp[4]  = -m_Elements[4]  * m_Elements[10] * m_Elements[15] +
                    m_Elements[4]  * m_Elements[11] * m_Elements[14] +
                    m_Elements[8]  * m_Elements[6]  * m_Elements[13] -
                    m_Elements[8]  * m_Elements[7]  * m_Elements[12] -
                    m_Elements[12] * m_Elements[6]  * m_Elements[11] +
                    m_Elements[12] * m_Elements[7]  * m_Elements[10];
        
        temp[8]  =  m_Elements[4]  * m_Elements[9]  * m_Elements[15] -
                    m_Elements[4]  * m_Elements[11] * m_Elements[13] -
                    m_Elements[8]  * m_Elements[5]  * m_Elements[14] +
                    m_Elements[8]  * m_Elements[7]  * m_Elements[13] +
                    m_Elements[12] * m_Elements[5]  * m_Elements[10] -
                    m_Elements[12] * m_Elements[7]  * m_Elements[9];

        temp[12] = -m_Elements[4]  * m_Elements[9]  * m_Elements[14] +
                    m_Elements[4]  * m_Elements[10] * m_Elements[13] +
                    m_Elements[8]  * m_Elements[5]  * m_Elements[14] -
                    m_Elements[8]  * m_Elements[6]  * m_Elements[13] -
                    m_Elements[12] * m_Elements[5]  * m_Elements[10] +
                    m_Elements[12] * m_Elements[6]  * m_Elements[9];

        temp[1]  = -m_Elements[1]  * m_Elements[10] * m_Elements[15] +
                    m_Elements[1]  * m_Elements[11] * m_Elements[14] +
                    m_Elements[9]  * m_Elements[2]  * m_Elements[15] -
                    m_Elements[9]  * m_Elements[3]  * m_Elements[14] -
                    m_Elements[13] * m_Elements[2]  * m_Elements[11] +
                    m_Elements[13] * m_Elements[3]  * m_Elements[10];

        temp[5]  =  m_Elements[0]  * m_Elements[10] * m_Elements[15] -
                    m_Elements[0]  * m_Elements[11] * m_Elements[14] -
                    m_Elements[8]  * m_Elements[2]  * m_Elements[15] +
                    m_Elements[8]  * m_Elements[3]  * m_Elements[14] +
                    m_Elements[12] * m_Elements[2]  * m_Elements[11] -
                    m_Elements[12] * m_Elements[3]  * m_Elements[10];

        temp[9]  = -m_Elements[0]  * m_Elements[9]  * m_Elements[15] +
                    m_Elements[0]  * m_Elements[11] * m_Elements[13] +
                    m_Elements[8]  * m_Elements[1]  * m_Elements[15] -
                    m_Elements[8]  * m_Elements[3]  * m_Elements[13] -
                    m_Elements[12] * m_Elements[1]  * m_Elements[11] +
                    m_Elements[12] * m_Elements[3]  * m_Elements[9];

        temp[13] =  m_Elements[0]  * m_Elements[9]  * m_Elements[14] -
                    m_Elements[0]  * m_Elements[10] * m_Elements[13] -
                    m_Elements[8]  * m_Elements[1]  * m_Elements[14] +
                    m_Elements[8]  * m_Elements[2]  * m_Elements[13] +
                    m_Elements[12] * m_Elements[1]  * m_Elements[10] -
                    m_Elements[12] * m_Elements[2]  * m_Elements[9];

        temp[2]  =  m_Elements[1]  * m_Elements[6]  * m_Elements[15] -
                    m_Elements[1]  * m_Elements[7]  * m_Elements[14] -
                    m_Elements[5]  * m_Elements[2]  * m_Elements[15] +
                    m_Elements[5]  * m_Elements[3]  * m_Elements[14] +
                    m_Elements[13] * m_Elements[2]  * m_Elements[7]  -
                    m_Elements[13] * m_Elements[3]  * m_Elements[6];

        temp[6]  = -m_Elements[0]  * m_Elements[6]  * m_Elements[15] +
                    m_Elements[0]  * m_Elements[7]  * m_Elements[14] +
                    m_Elements[4]  * m_Elements[2]  * m_Elements[15] -
                    m_Elements[4]  * m_Elements[3]  * m_Elements[14] -
                    m_Elements[12] * m_Elements[2]  * m_Elements[7]  +
                    m_Elements[12] * m_Elements[3]  * m_Elements[6];

        temp[10] =  m_Elements[0]  * m_Elements[5]  * m_Elements[15] -
                    m_Elements[0]  * m_Elements[7]  * m_Elements[13] -
                    m_Elements[4]  * m_Elements[1]  * m_Elements[15] +
                    m_Elements[4]  * m_Elements[3]  * m_Elements[13] +
                    m_Elements[12] * m_Elements[1]  * m_Elements[7]  -
                    m_Elements[12] * m_Elements[3]  * m_Elements[5];

        temp[14] = -m_Elements[0]  * m_Elements[5]  * m_Elements[14] +
                    m_Elements[0]  * m_Elements[6]  * m_Elements[13] +
                    m_Elements[4]  * m_Elements[1]  * m_Elements[14] -
                    m_Elements[4]  * m_Elements[2]  * m_Elements[13] -
                    m_Elements[12] * m_Elements[1]  * m_Elements[6]  +
                    m_Elements[12] * m_Elements[2]  * m_Elements[5];

        temp[3]  = -m_Elements[1]  * m_Elements[6]  * m_Elements[11] +
                    m_Elements[1]  * m_Elements[7]  * m_Elements[10] +
                    m_Elements[5]  * m_Elements[2]  * m_Elements[11] -
                    m_Elements[5]  * m_Elements[3]  * m_Elements[10] -
                    m_Elements[9]  * m_Elements[2]  * m_Elements[7]  +
                    m_Elements[9]  * m_Elements[3]  * m_Elements[6];
                    
        temp[7]  =  m_Elements[0]  * m_Elements[6]  * m_Elements[11] -
                    m_Elements[0]  * m_Elements[7]  * m_Elements[10] -
                    m_Elements[4]  * m_Elements[2]  * m_Elements[11] +
                    m_Elements[4]  * m_Elements[3]  * m_Elements[10] +
                    m_Elements[8]  * m_Elements[2]  * m_Elements[7]  -
                    m_Elements[8]  * m_Elements[3]  * m_Elements[6];

        temp[11] = -m_Elements[0]  * m_Elements[5]  * m_Elements[11] +
                    m_Elements[0]  * m_Elements[7]  * m_Elements[9]  +
                    m_Elements[4]  * m_Elements[1]  * m_Elements[11] -
                    m_Elements[4]  * m_Elements[3]  * m_Elements[9]  -
                    m_Elements[8]  * m_Elements[1]  * m_Elements[7]  +
                    m_Elements[8]  * m_Elements[3]  * m_Elements[5];

        temp[15] =  m_Elements[0]  * m_Elements[5]  * m_Elements[10] -
                    m_Elements[0]  * m_Elements[6]  * m_Elements[9]  -
                    m_Elements[4]  * m_Elements[1]  * m_Elements[10] +
                    m_Elements[4]  * m_Elements[2]  * m_Elements[9]  +
                    m_Elements[8]  * m_Elements[1]  * m_Elements[6]  -
                    m_Elements[8]  * m_Elements[2]  * m_Elements[6];

        float det = m_Elements[0] * temp[0] + m_Elements[1] * temp[4] + m_Elements[2] * temp[8] + m_Elements[3] * temp[12];
        det = ((float) 1) / det;

        for (int32_t i = 0; i < 4 * 4; i++)
            m_Elements[i] = temp[i] * det;

        return *this;
    }

    const float* mat4::GetElements() const 
    {
        return m_Elements;
    }

    const vec4& mat4::GetRow(int index) const
    {
        return m_Rows[index];
    }


    
    vec4 mat4::GetColumn(int32_t index) const
    {
        return vec4(m_Elements[index + 0 * 4], m_Elements[index + 1 * 4], m_Elements[index + 2 * 4], m_Elements[index + 3 * 4]);
    }

    
    void mat4::SetColumn(uint32_t index, const vec4& column)
    {
        m_Elements[index + 0 * 4] = column.GetX();
        m_Elements[index + 1 * 4] = column.GetY();
        m_Elements[index + 2 * 4] = column.GetZ();
        m_Elements[index + 3 * 4] = column.GetW();
    }

    vec4 mat4::GetPosition() const
    {
        return vec4(GetColumn(3));
    }

    void mat4::SetPosition(const vec4& position)
    {
        SetColumn(3, position);
    }

    
    mat4 mat4::Orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        mat4 result(1.0f);

        result.m_Elements[0 + 0 * 4] = 2.0f / (right - left  );
        
        result.m_Elements[0 + 1 * 4] = 2.0f / (top   - bottom);
        
        result.m_Elements[0 + 2 * 4] = 2.0f / (near  - far   );
        
        result.m_Elements[0 + 0 * 4] = (right  + left) / (left   - right);
        result.m_Elements[0 + 1 * 4] = (bottom + top ) / (bottom - top  );
        result.m_Elements[0 + 2 * 4] = (far    + near) / (far    - near );

        return result;
    }

    
    mat4 mat4::Perspective(float fov, float aspectRatio, float near, float far)
    {
        mat4 result = Identity();

        float q = 1.0f / tan(toRadians(0.5f * fov));
        float a = q / aspectRatio;

        float b = (near + far) / (near - far);
        float c = (2 * near * far) / (near - far);

        result.m_Elements[0 + 0 * 4] = a;
        result.m_Elements[1 + 1 * 4] = q;
        result.m_Elements[2 + 2 * 4] = b;
        result.m_Elements[2 + 3 * 4] = -1.0f;
        result.m_Elements[3 + 2 * 4] = c;

        return result;
    }

    
    mat4 mat4::LookAt(const vec3& camera, const vec3& object, const vec3& up)
    {
        mat4 result = Identity();

        vec3 f = (object - camera).Normalize();



        return result;

    }

    mat4 mat4::Translate(const vec3& translation)
    {
        mat4 result(1.0f);
        result.m_Elements[3 + 0 * 4] = translation.GetX();
        result.m_Elements[3 + 1 * 4] = translation.GetY();
        result.m_Elements[3 + 2 * 4] = translation.GetZ();
        return result;
    }

    mat4 mat4::Rotate(float angle, const vec3& axis)
    {
        mat4 result(1.0f);

        float r = toRadians(angle);
        float c = cos(r);
        float s = sin(r);
        float omc = 1.0f - c;

        float x = axis.GetX();
        float y = axis.GetY();
        float z = axis.GetZ();

        result.m_Elements[0 + 0 * 4] = x * x * omc + c;
        result.m_Elements[0 + 1 * 4] = x * y * omc + z * s;
        result.m_Elements[0 + 2 * 4] = x * z * omc - y * s;

        result.m_Elements[1 + 0 * 4] = x * y * omc - z * s;
        result.m_Elements[1 + 1 * 4] = y * y * omc + c;
        result.m_Elements[1 + 2 * 4] = y * z * omc + x * s;
        
        result.m_Elements[2 + 0 * 4] = x * z * omc + y * s;
        result.m_Elements[2 + 1 * 4] = y * z * omc - x * s;
        result.m_Elements[2 + 2 * 4] = z * z * omc + c;

        return result;
    }

    mat4 mat4::Scale(const vec3& scale)
    {
        mat4 result(1.0f);

        result.m_Elements[0 + 0 * 4] = scale.GetX();
        result.m_Elements[1 + 1 * 4] = scale.GetY();
        result.m_Elements[2 + 2 * 4] = scale.GetZ();

        return result;
    }

    mat4 mat4::Invert(const mat4& matrix)
    {
        mat4 result = matrix;
        return result.Invert();
    }

    mat4 mat4::Transpose(const mat4& matrix)
    {
        return mat4(
            vec4(matrix.GetRow(0).GetX(), matrix.GetRow(1).GetX(), matrix.GetRow(2).GetX(), matrix.GetRow(3).GetX()),
			vec4(matrix.GetRow(0).GetY(), matrix.GetRow(1).GetY(), matrix.GetRow(2).GetY(), matrix.GetRow(3).GetY()),
			vec4(matrix.GetRow(0).GetZ(), matrix.GetRow(1).GetZ(), matrix.GetRow(2).GetZ(), matrix.GetRow(3).GetZ()),
			vec4(matrix.GetRow(0).GetW(), matrix.GetRow(1).GetW(), matrix.GetRow(2).GetW(), matrix.GetRow(3).GetW())
        );
    }
    
};