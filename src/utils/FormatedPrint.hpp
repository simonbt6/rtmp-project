/**
 * Author: Simon Brisebois-Therrien 
 * Date: 2021-08-21
 **/
#pragma once

#include <iostream>
#include <string>

#define __DEBUG true
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
        template <typename T = int>
        static void PrintBytes(T* arr, int length)
        {
            #ifdef DEBUG
            for (int i = 0; i < length; i++)
            {
                if (i % 16 == 0) printf("\n");
                else if (i % 8 == 0) printf("   ");

                printf("%X  ", arr[i]);

                if (arr[i] < 0x10) printf(" ");
            }
            #endif
        }
        enum class Color
        {
            Black,
            Red,
            Green,
            Yellow,
            Blue,
            Purple,
            Cyan,
            White,
            Reset
        };

        static inline const string colorMarker[9] = {
            "\033[0;30m",
            "\033[0;31m",
            "\033[0;32m",
            "\033[0;33m",
            "\033[0;34m",
            "\033[0;35m",
            "\033[0;36m",
            "\033[0;37m",
            "\033[0;0m"
        };

        static void PrintFormated(string sender, string text, Color color = Color::White)
        {
            #if __DEBUG == true
            int indexSemiColon = sender.find("::");
            if (indexSemiColon > 0)
            {
                string greenMarker = colorMarker[(int) Color::Green]; 
                sender.insert(sender.begin(), greenMarker.begin(), greenMarker.end());

                string resetMarker = colorMarker[(int) Color::Reset];
                sender.insert(sender.begin() + indexSemiColon + greenMarker.length(), resetMarker.begin(), resetMarker.end());

                string marker = colorMarker[(int) Color::Blue];
                sender.insert(sender.begin() + indexSemiColon + greenMarker.length() + resetMarker.length() + 2, marker.begin(), marker.end());
            }

            printf(
                "\n[%s%s%s] %s%s%s", 
                colorMarker[(int) Color::Red].c_str(), 
                sender.c_str(), 
                colorMarker[(int) Color::Reset].c_str(), 
                colorMarker[(int) color].c_str(), 
                text.c_str(),
                colorMarker[(int) Color::Reset].c_str());
            #endif
        }

        static void PrintError(string sender, string text)
        {
            PrintFormated(sender, text, Color::Red);
        }

        static void PrintInfo(string text)
        {
            PrintFormated("INFO", text, Color::Yellow);
        }
    };
}