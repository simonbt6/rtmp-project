#include "mat4.hpp"

namespace Graphics
{
    namespace Maths
    {
        template<typename IntegerType>
        mat4<IntegerType>::mat4()
        {

        }

        template<typename IntegerType>
        mat4<IntegerType>::mat4(IntegerType diagonal)
        {
            
        }

        template<typename IntegerType>
        mat4<IntegerType>::mat4(IntegerType* elements)
        {
            
        }

        template<typename IntegerType>
        mat4<IntegerType>::mat4(const vec4<IntegerType>& r0, const vec4<IntegerType>& r1, const vec4<IntegerType>& r2, const vec4<IntegerType>& r3)
        {
            
        }

        template<typename IntegerType>
        mat4<IntegerType> mat4<IntegerType>::Identity()
        {
            return mat4<IntegerType>((IntegerType)1);
        }

        template<typename IntegerType>
        mat4<IntegerType>& mat4<IntegerType>::Multiply(const mat4<IntegerType>& other)
        {

        }

        template<typename IntegerType>
        mat4<IntegerType> operator*(mat4<IntegerType>& left, const mat4<IntegerType>& right)
        {
            
        }

        template<typename IntegerType>
        mat4<IntegerType>& mat4<IntegerType>::operator*=(const mat4<IntegerType>& other)
        {
            
        }

        template<typename IntegerType>
        vec3<IntegerType> mat4<IntegerType>::Multiply(const vec3<IntegerType>& other) const
        {

        }

        template<typename IntegerType>
        vec3<IntegerType> operator*(const mat4<IntegerType>& left, const vec3<IntegerType>& right)
        {

        }

        template<typename IntegerType>
        vec4<IntegerType> mat4<IntegerType>::Multiply(const vec4<IntegerType>& other) const
        {

        }

        template<typename IntegerType>
        vec4<IntegerType> operator*(const mat4<IntegerType>& left, const vec4<IntegerType>& right)
        {

        }

        template<typename IntegerType>
        mat4<IntegerType>& mat4<IntegerType>::Invert()
        {

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
};