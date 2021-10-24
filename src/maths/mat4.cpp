#include <maths/mat4.hpp>

namespace Maths
{
    template<typename IntegerType>
    mat4<IntegerType>::mat4()
    {
        memset(m_Elements, 0, 4 * 4 * sizeof(IntegerType));
    }

    template<typename IntegerType>
    mat4<IntegerType>::mat4(IntegerType diagonal)
    {
        memset(m_Elements, 0, 4 * 4 * sizeof(IntegerType));
        m_Elements[0 + 0 * 4] = diagonal;
        m_Elements[1 + 1 * 4] = diagonal;
        m_Elements[2 + 2 * 4] = diagonal;
        m_Elements[3 + 3 * 4] = diagonal;
    }

    template<typename IntegerType>
    mat4<IntegerType>::mat4(IntegerType* elements)
    {
        memcpy(this->m_Elements, elements, 4 * 4 * sizeof(IntegerType));   
    }

    template<typename IntegerType>
    mat4<IntegerType>::mat4(IntegerType n0, IntegerType n1, IntegerType n2, IntegerType n3)
    {
        m_Rows[0] = Maths::vec4(n0);
        m_Rows[1] = Maths::vec4(n1);
        m_Rows[2] = Maths::vec4(n2);
        m_Rows[3] = Maths::vec4(n3);
    }

    template<typename IntegerType>
    mat4<IntegerType>::mat4(const vec4<IntegerType>& r0, const vec4<IntegerType>& r1, const vec4<IntegerType>& r2, const vec4<IntegerType>& r3)
    {
        m_Rows[0] = r0;
        m_Rows[1] = r1;   
        m_Rows[2] = r2;   
        m_Rows[3] = r3;   
    }

    template<typename IntegerType>
    mat4<IntegerType> mat4<IntegerType>::Identity()
    {
        return mat4<IntegerType>((IntegerType)1);
    }

    template<typename IntegerType>
    mat4<IntegerType>& mat4<IntegerType>::Multiply(const mat4<IntegerType>& other)
    {
        IntegerType data[16];
        for (int32_t row = 0; row < 4; row++)
        {
            for (int32_t col = 0; col < 4; col++)
            {
                IntegerType sum = (IntegerType) 0;
                for (int32_t e = 0; e < 4; e++)
                    sum += m_Elements[e + row * 4] * other.GetElements()[col + e * 4];
                data[col + row * 4] = sum;
                }
        }
        memcpy(m_Elements, data, 4 * 4 * sizeof(IntegerType));
        return *this;
    }

    template<typename IntegerType>
    mat4<IntegerType> operator*(mat4<IntegerType>& left, const mat4<IntegerType>& right)
    {
        return left.Multiply(right);
    }

    template<typename IntegerType>
    mat4<IntegerType>& mat4<IntegerType>::operator*=(const mat4<IntegerType>& other)
    {
        return Multiply(other);
    }

    template<typename IntegerType>
    vec3<IntegerType> mat4<IntegerType>::Multiply(const vec3<IntegerType>& other) const
    {
        return other.Multiply(*this);
    }

    template<typename IntegerType>
    vec3<IntegerType> operator*(const mat4<IntegerType>& left, const vec3<IntegerType>& right)
    {
        return left.Multiply(right);
    }

    template<typename IntegerType>
    vec4<IntegerType> mat4<IntegerType>::Multiply(const vec4<IntegerType>& other) const
    {
        return other.Multiply(*this);
    }

    template<typename IntegerType>
    vec4<IntegerType> operator*(const mat4<IntegerType>& left, const vec4<IntegerType>& right)
    {
        return left.Multiply(right);
    }

    template<typename IntegerType>
    mat4<IntegerType>& mat4<IntegerType>::Invert()
    {
        IntegerType temp[16];

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

        IntegerType det = m_Elements[0] * temp[0] + m_Elements[1] * temp[4] + m_Elements[2] * temp[8] + m_Elements[3] * temp[12];
        det = ((IntegerType) 1) / det;

        for (int32_t i = 0; i < 4 * 4; i++)
            m_Elements[i] = temp[i] * det;

        return *this;
    }

    template<typename IntegerType>
    vec4<IntegerType> mat4<IntegerType>::GetColumn(int32_t index) const
    {
        return vec4<IntegerType>(m_Elements[index + 0 * 4], m_Elements[index + 1 * 4], m_Elements[index + 2 * 4], m_Elements[index + 3 * 4]);
    }

    template<typename IntegerType>
    void mat4<IntegerType>::SetColumn(uint32_t index, const vec4<IntegerType>& column)
    {
        m_Elements[index + 0 * 4] = column.GetX();
        m_Elements[index + 1 * 4] = column.GetY();
        m_Elements[index + 2 * 4] = column.GetZ();
        m_Elements[index + 3 * 4] = column.GetW();
    }

    template<typename IntegerType>
    mat4<IntegerType> mat4<IntegerType>::Orthographic(IntegerType left, IntegerType right, IntegerType bottom, IntegerType top, IntegerType near, IntegerType far)
    {
        mat4<IntegerType> result((IntegerType) 1);

        result.m_Elements[0 + 0 * 4] = (IntegerType) 2 / (right - left  );
        
        result.m_Elements[0 + 1 * 4] = (IntegerType) 2 / (top   - bottom);
        
        result.m_Elements[0 + 2 * 4] = (IntegerType) 2 / (near  - far   );
        
        result.m_Elements[0 + 0 * 4] = (right  + left) / (left   - right);
        result.m_Elements[0 + 1 * 4] = (bottom + top ) / (bottom - top  );
        result.m_Elements[0 + 2 * 4] = (far    + near) / (far    - near );

        return result;
    }

    template<typename IntegerType>
    mat4<IntegerType> mat4<IntegerType>::Perspective(IntegerType fov, IntegerType aspectRatio, IntegerType near, IntegerType far)
    {
        mat4<IntegerType> result = Identity();

        IntegerType q = (IntegerType)( 1 / tan(toRadians(0.5f * fov)));
        IntegerType a = (IntegerType)( q / aspectRatio);

        IntegerType b = (IntegerType)((near + far) / (near - far));
        IntegerType c = (IntegerType)((2 * near * far) / (near - far));

        result.m_Elements[0 + 0 * 4] = a;
        result.m_Elements[1 + 1 * 4] = q;
        result.m_Elements[2 + 2 * 4] = b;
        result.m_Elements[2 + 3 * 4] = (IntegerType)-1;
        result.m_Elements[3 + 2 * 4] = c;

        return result;
    }

    template<typename IntegerType>
    mat4<IntegerType> mat4<IntegerType>::LookAt(const vec3<IntegerType>& camera, const vec3<IntegerType>& object, const vec3<IntegerType>& up)
    {
        mat4<IntegerType> result = Identity();

        vec3<IntegerType> f = (object - camera).Normalize();

    }
    
};