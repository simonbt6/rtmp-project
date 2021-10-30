#pragma once

/**
 * @author Simon Brisebois-Therrien (simonbt6@gmail.com)
 * @date 2021-10-02
 * 
 * 
 */

#include <iostream>
#include <vector>

#include "FormatedPrint.hpp"


namespace Utils
{
    class MemoryManagement
    {
        public:

            template <typename T>
            static void FreeVector(std::vector<T> vector);
    };
};