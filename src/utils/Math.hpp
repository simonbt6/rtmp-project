#pragma once
/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-24
 * 
 * Math library
 **/
#include "Bit.hpp"
#include <vector>
#include <cmath>

namespace Utils 
{
    class Math
    {
        private:
            static double CalculateMantissa(vector<unsigned char>& bits, int low, int high)
            {
                double mantissa = 0.00;
                for (int i = 0; i < (high - low + 1); i++)
                    mantissa += ((double)(bits.at(i + low)))*pow(2, -(i + 1));
                return mantissa;
            }
            
        public:



            static double IE754ToDouble(unsigned char* bytes)
            {
                vector<unsigned char> bits = BitOperations::BytesToBits(bytes);
                double mantissa;
                unsigned int
                    exponent, 
                    sign = bits[0], 
                    bias = 1023;                
                mantissa = CalculateMantissa(bits, 12, 63);
                exponent = BitOperations::BitsToInteger(bits, 1, 11);

                double dValue = std::pow(-1, sign) * std::pow(2, (exponent - bias)) * (1.0 + mantissa);
                return 
                    (dValue == std::numeric_limits<double>::infinity())? 1 : dValue;
            }
    };
}