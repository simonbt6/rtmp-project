#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <iomanip>
//#include "../core/Netconnection.hpp"
#include "Bit.hpp"
#include "Math.hpp"
using namespace std;

/**
 * Type defintions
 **/

#define KB (1 << 10)
#define MB (1 << 20)
#define GB (1 << 30)



namespace Utils
{
    
    class AMF0Decoder
    {
        private:
            typedef char U8;
            typedef unsigned short int U16;
            typedef short int S16;
            typedef unsigned long int U32;
            typedef double DOUBLE;

            enum type_markers
            {
                number_marker       = 0x00,
                boolean_marker      = 0x01,
                string_marker       = 0x02,
                object_marker       = 0x03,
                movieclips_marker   = 0x04,          // Not supported
                null_marker         = 0x05,
                undefined_marker    = 0x06,
                referenced_marker   = 0x07,
                ecma_marker         = 0x08,
                object_end_marker   = 0x09,
                strict_array_marker = 0x0A,
                date_marker         = 0x0B,
                long_string_marker  = 0x0C,
                unsupported_marker  = 0x0D,
                recordset_marker    = 0x0E,
                xml_document_marker = 0x0F,         // Not supported
                typed_object_marker = 0x10,
                avm_object_marker   = 0x11
            };
            static int FindIndex(unsigned char* bytes, int size, type_markers marker)
            {
                for (int i = 0; i < size; i++)
                    if(bytes[i] == marker)
                        return i;
                return -1;
            }

            static unsigned char* Get(unsigned char* bytes, int count, int startingIndex)
            {
                unsigned char* arr = new unsigned char[count];
                for (int i = 0; i < count; i++)
                    arr[i] = bytes[i + startingIndex];
                return arr;
            }

            


        public:
            static void DecodeCommand(
                unsigned char* bytes, 
                int size
                /*ClientCommand& command*/)
            {

                vector<int> markerIndexes;

                /**
                 * Command Name
                 **/
                int commandNameMarkerIndex = 
                    FindIndex(bytes, size, type_markers::string_marker);
                int commandNameLength;
                unsigned char* commandName;

                /**
                 * Transaction ID
                 **/
                int transactionID;
                int transactionIDIndex;
                unsigned char* transactionIDArray;

                /**
                 * Object
                 **/
                unsigned char* data; 

                if (bytes[commandNameMarkerIndex + 1] == 0x00)
                {
                    commandNameLength = bytes[commandNameMarkerIndex + 2];
                    printf("\nCommand Name: ");
                    commandName = Get(
                            bytes, 
                            commandNameLength, 
                            commandNameMarkerIndex + 3);
                    for (int i = 0; i < commandNameLength; i++)
                    {
                        printf("%c", commandName[i]);
                    }
                    /**
                     * Transaction ID
                     **/
                    transactionIDIndex = commandNameMarkerIndex + commandNameLength + 3;
                    if (bytes[transactionIDIndex] == type_markers::number_marker)
                    {
                        transactionIDArray = Get(bytes, 8, transactionIDIndex + 1);
                        transactionID = Utils::Math::IE754ToInt(transactionIDArray);

                        printf("\nTransaction ID: %i", transactionID);
                    }
                }

                printf("\n");
            };
    };
}