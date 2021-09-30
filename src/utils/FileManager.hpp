#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-09-30 
 * 
 * 
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

namespace Utils 
{
    class FileManager
    {
        public:
            static std::vector<std::string>* ReadLinesFromFile(std::string filename);

            static std::string ReadStringFromFile(std::string filename);

            static std::vector<unsigned char>* ReadBytesFromFile(std::string filename);


    };
};