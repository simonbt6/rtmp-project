#pragma once
/**
 * @author Simon Brisebois-Therrien
 * @since  2021-09-09
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace Utils
{
    class LOGS
    {
        private:
            static string GenerateUniqueName()
            {
                unsigned char r = ((char)rand() % 256);
                string filename = "output_";
                filename += std::to_string(r);
                filename += ".log";
                return filename;
            }

        public:
            static void Log(unsigned char* data, int size)
            {
                string filename = GenerateUniqueName();
                ofstream out("logs/" + filename);
                
                for (int i = 0; i < size; i++)
                {
                    if (i % 16 == 0) out << "\n";
                    if (i % 8 == 0) out << "   ";

                    out << std::hex << std::setfill('0') << int(data[i]) << " ";

                    if (data[i] < 0x10) printf(" ");
                }

            }
    };
}