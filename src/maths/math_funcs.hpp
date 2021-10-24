#pragma once

#include <math.h>
#include <inttypes.h>

#define MATH_PI 3.14159265358f

namespace Maths
{
    inline float toRadians(float degrees) { return (float)(degrees * (MATH_PI / 180.0f)); }

    inline float toDegrees(float radians) { return (float)(radians * (180.0f / MATH_PI)); }

    inline int32_t sign(float value) { return (value > 0) - (value < 0); }

    inline float sin(float angle)   { return ::sin(angle); }

    inline float cos(float angle)   { return ::cos(angle); }

    inline float tan(float angle)   { return ::tan(angle); }

    inline float sqrt(float value)  { return ::sqrt(value); }

    inline float rsqrt(float value) { return 1.0f / ::sqrt(value); }

    inline float asin(float value)  { return ::asin(value); }

    inline float acos(float value)  { return ::acos(value); }

    inline float atan(float value)  { return ::atan(value); }

    inline float atan2(float y, float x) { return ::atan2(y, x); }

    inline float minV(float value, float min) { return (value < min) ? min : value; }

    inline float maxV(float value, float max) { return (value > max) ? max : value; }

    inline float clamp(float value, float min, float max) { return (value > min) ? (value < max) ? value: max : min; }

}