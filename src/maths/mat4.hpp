#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-15 
 * 
 * 
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "vec3.hpp"
#include "vec4.hpp"
#include "math_funcs.hpp"

namespace Maths
{
    class mat4
    {
        private:
            union
            {
                float m_Elements[4 * 4];
                vec4  m_Rows[4]; 
            };

        public:

            mat4();
            mat4(float diagonal);
            mat4(float* elements);
            mat4(float n0, float n1, float n2, float n3);
            mat4(const vec4& r0, const vec4& r1, const vec4& r2, const vec4& r3);

            ~mat4();

            static mat4 Identity();
            
            mat4& operator=(const mat4& other);

            mat4& Multiply   (const mat4& other);
            friend mat4 operator*(mat4 left, const mat4& right);
            mat4& operator*=(const mat4& other);

            vec3 Multiply(const vec3& other) const;
            friend vec3 operator*(const mat4& left, const vec3& right);

            vec4 Multiply(const vec4& other) const;
            friend vec4 operator*(const mat4& left, const vec4& right);

            mat4& Invert();

            const float* GetElements() const;

            const vec4& GetRow(int index) const;

            vec4 GetColumn(int32_t index) const;
            void SetColumn(uint32_t index, const vec4& column);
            vec4 GetPosition() const;
            void SetPosition(const vec4& position);

            static mat4 Orthographic(float left, float right, float bottom, float top, float near, float far);
            static mat4 Perspective (float fov, float aspectRatio, float near, float far);
            static mat4 LookAt      (const vec3& camera, const vec3& object, const vec3& up);

            static mat4 Translate(const vec3& translation);
            static mat4 Rotate   (float angle, const vec3& axis);
            // static mat4 Rotate   (const Quaternion& quat);
            static mat4 Scale    (const vec3& scale);
            static mat4 Invert   (const mat4& matrix);

            static mat4 Transpose(const mat4& matrix);

            // String toString() const;



    };
};