#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-15 
 * 
 * 
 */

#include <inttypes.h>

#include "vec3.hpp"
#include "vec4.hpp"
#include "math_funcs.hpp"

namespace Maths
{
    template <typename IntegerType>
    class mat4
    {
        private:
            union
            {
                IntegerType m_Elements[4 * 4];
                vec4<IntegerType> m_Rows[4]; 
            };

        public:

            mat4();
            mat4(IntegerType diagonal);
            mat4(IntegerType* elements);
            mat4(IntegerType n0, IntegerType n1, IntegerType n2, IntegerType n3);
            mat4(const vec4<IntegerType>& r0, const vec4<IntegerType>& r1, const vec4<IntegerType>& r2, const vec4<IntegerType>& r3);

            ~mat4() {};

            static mat4 Identity();

            mat4<IntegerType>& Multiply   (const mat4<IntegerType>& other);
            friend mat4<IntegerType> operator*(mat4<IntegerType> left, const mat4<IntegerType>& right);
            mat4<IntegerType>& operator*=(const mat4<IntegerType>& other);

            vec3<IntegerType> Multiply(const vec3<IntegerType>& other) const;
            friend vec3<IntegerType> operator*(const mat4<IntegerType>& left, const vec3<IntegerType>& right);

            vec4<IntegerType> Multiply(const vec4<IntegerType>& other) const;
            friend vec4<IntegerType> operator*(const mat4<IntegerType>& left, const vec4<IntegerType>& right);

            mat4<IntegerType>& Invert();

            inline IntegerType* GetElements() const{ return m_Elements; }

            vec4<IntegerType> GetColumn(int32_t index) const;
            void SetColumn(uint32_t index, const vec4<IntegerType>& column);
            inline vec3<IntegerType> GetPosition() const { return vec3<IntegerType>(GetColumn(3)); }
            inline void SetPosition(const vec3<IntegerType>& position) { SetColumn(3, vec4(position, 1.0f)); }

            static mat4<IntegerType> Orthographic(IntegerType left, IntegerType right, IntegerType bottom, IntegerType top, IntegerType near, IntegerType far);
            static mat4<IntegerType> Perspective (IntegerType fov, IntegerType aspectRatio, IntegerType near, IntegerType far);
            static mat4<IntegerType> LookAt      (const vec3<IntegerType>& camera, const vec3<IntegerType>& object, const vec3<IntegerType>& up);

            static mat4<IntegerType> Translate(const vec3<IntegerType>& translation);
            static mat4<IntegerType> Rotate   (IntegerType angle, const vec3<IntegerType>& axis);
            // static mat4<IntegerType> Rotate   (const Quaternion& quat);
            static mat4<IntegerType> Scale    (const vec3<IntegerType>& scale);
            static mat4<IntegerType> Invert   (const mat4<IntegerType>& matrix);

            static mat4<IntegerType> Transpose(const mat4<IntegerType>& matrix);

            // String toString() const;



    };
};