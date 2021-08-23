#pragma once
#include <cstdint>

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
                result = (result << 8) +bits[ n ];
            else
                for (unsigned n = 0; n < count; n++)
                result = (result << 8) +bits[ n ];
            return result;
        }

        static int concatenateBigEndian(int result, unsigned char* bytes, int count)
        {
            for (int i = 0; i < count; i++)
            {
                result = (result << 8) + bytes[i];
            }
            return result;
        }        
    };

        
}