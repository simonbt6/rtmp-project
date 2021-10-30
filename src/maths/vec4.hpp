#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-13 
 * 
 * 
 */

#include "vec3.hpp"
#include "math_funcs.hpp"

namespace Maths
{
    class mat4;
    
    class vec4
    {
        protected:
            float m_X;
            float m_Y;
            float m_Z;
            float m_W;

        public:
            vec4(float value);
            vec4(float x, float y, float z, float w);
            vec4(vec3 xyz, float w);
            ~vec4();

            // Getters and setters.
            float GetX() const;
            float GetY() const;
            float GetZ() const;
            float GetW() const;

            void SetX(float value);
            void SetY(float value);
            void SetZ(float value);
            void SetW(float value);


            // Math operations
            vec4 Add        (const vec4& other)  const;
            vec4 Substract  (const vec4& other)  const;
            vec4 Multiply   (const vec4& other)  const;
            vec4 Multiply   (const mat4& matrix) const;
            vec4 Divide     (const vec4& other)  const;


            vec4& SetEqualTo (const vec4& other);
            vec4& operator=  (const vec4& other);

            vec4  operator+  (const vec4& other) const;
            vec4& operator+= (const vec4& other);
            vec4& operator++ ();

            vec4  operator-  (const vec4& other) const;
            vec4& operator-= (const vec4& other);
            vec4& operator--  ();

            vec4  operator*  (const vec4& other) const;
            vec4& operator*= (const vec4& other);

            vec4  operator/  (const vec4& other) const;
            vec4& operator/= (const vec4& other);



            // Logical operations
            bool IsEqualTo         (const vec4& other) const;
            bool IsNotEqualTo      (const vec4& other) const;

            bool IsGreaterThan     (const vec4& other) const;
            bool IsGreaterOrEqualTo(const vec4& other) const;
            
            bool IsLessThan        (const vec4& other) const;
            bool IsLessOrEqualTo   (const vec4& other) const;
            

            
            bool operator==(const vec4& other) const;
            bool operator!=(const vec4& other) const;
            
            bool operator> (const vec4& other) const;
            bool operator>=(const vec4& other) const;

            bool operator< (const vec4& other) const;
            bool operator<=(const vec4& other) const;

    };
};