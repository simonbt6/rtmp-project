#pragma once
#include <cstdint>
#include <vector>
#include <random>
#include <math.h>

using namespace std;

namespace Utils
{
    class BitOperations
    {
        public:
        template <typename IntegerType>
        static IntegerType bytesToInteger( 
            IntegerType& result, 
            const unsigned char* bits, 
            bool little_endian = true,
            int count = 4)
        {
            result = 0;
            if (little_endian)
                for (int n = count; n >= 0; n--)
                result = (result << 8) + bits[ n ];
            else
                for (unsigned n = 0; n < count; n++)
                result = (result << 8) + bits[ n ];
            return result;
        }

        static vector<unsigned char> BytesToBits(unsigned char* bytes)
        {
            vector<unsigned char> bits;
            for (int n = 0; n < sizeof(bytes); n++)
                for (int i = 7; i >= 0; i--)
                    bits.push_back((((1 << (i % 8)) & (bytes[n])) >> (i%8)));
            return bits;
        }

        static unsigned int BitsToInteger(vector<unsigned char> bits, int low, int high)
        {
            unsigned int f = 0;
            for (int i = 0; i < (high - low + 1); i++)
                if(bits.at(i + low))
                    f |= 1 << i;
            return f;
        }

        static char* GenerateRandom8BitBytes(int size)
        {
            char* arr = new char[size];
            for (int i = 0; i < size; i++)
                arr[i] = abs(rand()%128);
            return arr;
        }  
    };

        
}