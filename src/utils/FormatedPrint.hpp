/**
 * Author: Simon Brisebois-Therrien 
 * Date: 2021-08-21
 **/
#pragma once

#include <iostream>

namespace Utils 
{
    class FormatedPrint
    {
        public:
        /**
         * Ex: 
         * FF FF FF FF FF FF FF FF   FF FF FF FF FF FF FF FF
         * FF FF FF FF FF FF FF FF   FF FF FF FF FF FF FF FF
         * FF FF FF FF FF FF FF FF   FF FF FF FF FF FF FF FF
         * ...
         **/
        static void PrintBytes(int* arr, int length)
        {
            for (int i = 0; i < length; i++)
            {
                if (i % 16 == 0) printf("\n");
                else if (i % 8 == 0) printf("   ");

                printf("%X  ", arr[i]);

                if (arr[i] < 0x10) printf(" ");
            }
        }
    };
}