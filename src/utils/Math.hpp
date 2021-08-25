#pragma once
/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-24
 * 
 * Math library
 **/
#include "Bit.hpp"
#include <vector>

namespace Utils 
{
    class Math
    {
        public:

            static int IE754ToInt(unsigned char* bytes)
            {
                vector<unsigned char> bits = BitOperations::BytesToBits(bytes);
                unsigned int
                    mantissa, 
                    exponent, 
                    sign = (bits[0])? -1 : 1, 
                    bias = 1023;                

                mantissa = BitOperations::BitsToInteger(bits, 12, 64);
                exponent = BitOperations::BitsToInteger(bits, 1, 11);

                double dValue = sign * pow(2, (exponent - bias)) * (1 + mantissa);

                return 
                    (dValue == std::numeric_limits<double>::infinity())? 1 : dValue;
            }
    };
}