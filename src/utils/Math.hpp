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

            static vector<int> BitsToBytes(vector<int>& bits)
            {
                vector<int> bytes;
                unsigned int byte = 0;
                for (int i = 0; i < bits.size(); i++)
                {
                    if (bits.at(i))
                        byte |= 1 << (i % 8);
                    if ((i + 1) % 8 == 0)
                    {
                        bytes.push_back(byte);
                        byte = 0;
                    }
                }
                reverse(bytes.begin(), bytes.end());
                return bytes;
            }
            
        public:

            static int* DoubleToIEEE754(double value)
            {
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

                // 8. Normalize mantissa
                bMantissa.erase(bMantissa.begin());
                int size = bMantissa.size();

                for (int i = 0; i < (52 - size); i++)
                    bMantissa.push_back(0);

                // 9.
                vector<int> bValue;
                bValue.push_back((value > 0)? 0 : 1);
                bValue.insert(bValue.end(), bExposant.begin(), bExposant.end());
                bValue.insert(bValue.end(), bMantissa.begin(), bMantissa.end());
                reverse(bValue.begin(), bValue.end());

                vector<int> bytes = BitsToBytes(bValue);

                printf("\n\nIEEE754 bytes: ");
                for (int v : bytes)
                    printf("%X ", v);
                printf("\n\n");
                
                return bytes.data();
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