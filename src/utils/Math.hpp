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
#include <algorithm>

using namespace std;

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

            static vector<int> Base2(int value)
            {
                vector<int> remainers;
                while (value != 0)
                {
                    remainers.push_back(value % 2);
                    value /= 2;
                }
                return remainers;
            }

            static char* BitsToBytes(vector<int>& bits)
            {
                char* bytes = new char[8];

                for (int i = 0; i < bits.size(); i += 8)
                {
                    char byte = 0;
                    int z = 7;
                    for (int n = 0; n < 8; n++)
                    {
                        if (bits.at(n + i))
                            byte += 1 << (z);
                        z--;
                    }
                    bytes[i / 8] = byte;
                    printf("\nByte value: %X", byte);
                }
                return bytes;
            }
            
        public:

            static char* DoubleToIEEE754(double value)
            {
                Utils::FormatedPrint::PrintFormated(
                    "Math::DoubleToIEEE754",
                    "Double value: " + to_string((int)value)
                );
                // 1.
                vector<int> bMantissa = Base2(value);

                // 2.
                reverse(bMantissa.begin(), bMantissa.end());

                // 3.
                int vExposant = bMantissa.size() - 1;

                // 5.
                vExposant = vExposant + std::pow(2, (11 - 1)) - 1;

                // 6.
                vector<int> bExposant = Base2(vExposant);
                reverse(bExposant.begin(), bExposant.end());
                bExposant.insert(bExposant.begin(), 11 - bExposant.size(), 0);


                // 8. Normalize mantissa
                bMantissa.erase(bMantissa.begin());

                // 9.
                vector<int> bValue;
                bValue.push_back((value > 0)? 0 : 1);
                bValue.insert(bValue.end(), bExposant.begin(), bExposant.end());
                bValue.insert(bValue.end(), bMantissa.begin(), bMantissa.end());
                bValue.insert(bValue.end(), 64 - bValue.size(), 0);
                // reverse(bValue.begin(), bValue.end());

                char* bytes = BitsToBytes(bValue);

                printf("\n\n[Math::DoubleToIEEE754] IEEE754 bytes: ");
                for (int i = 0; i < 8; i++) 
                    printf("%X ", bytes[i]);
                
                // TODO: Fix value 0 number.
                return value ? bytes : new char[8]{0,0,0,0,0,0,0,0};
            }

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