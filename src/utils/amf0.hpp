#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <iomanip>
#include <string>
//#include "../core/Netconnection.hpp"
#include "Bit.hpp"
#include "Math.hpp"
#include "FormatedPrint.hpp"
using namespace std;

/**
 * Type defintions
 **/

#define KB (1 << 10)
#define MB (1 << 20)
#define GB (1 << 30)



namespace Utils
{
    class AMF0
    {
        public:
            /**
             * Types
             **/
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
                reference_marker    = 0x07,
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

            /**
             * Number type.
             * 
             * Marker: number-marker. 
             * 
             * An AMF0 Number type is used to encode an ActionScript Number.
             * The data following Number marker is always an 8-byte IEEE754
             * double precision floating point value in network byte order.
             * 
             **/
            typedef struct
            {
                double value;
                unsigned char* data;

            } Number;

            /**
             * Boolean type.
             * 
             * Marker: boolean-marker.
             * 
             * An AMF0 Boolean type is used to encode a primitive ActionScript
             * 1.0 or 2.0 Boolean or an ActionScript 3.0 Boolean. The Object
             * (non-primitive) version of ActionScript 1.0 or 2.0 are not seria-
             * lizable. A Boolean type marker is followed by an unsigned byte;
             * a zero byte value denotes false while a non-zero byte value (i.e. 1)
             * denotes true.
             * 
             **/
            typedef struct
            {
                bool value;
                unsigned char* data;
            } Boolean;

            /**
             * String Type.
             * 
             * Marker: string-marker.
             * 
             * All strings in AMF are encoded using UTF-8; however, the byte-
             * length header format may vary. The AMF0 String type uses the
             * standard byte-length header (ie U16). For long Strings that 
             * require more than 65535 bytes to encode in UTF-8, the AMF0 Long
             * String type should be used.
             **/
            typedef struct
            {
                char* value;
                unsigned short int length;
                unsigned char* data;
            } String;

            /**
             * Object type.
             * 
             * The AMF0 Object type is used to encode anonymous ActionScript object.
             * Any typed object that does not have a registred class should be treated
             * as an anonymous ActionScript object. If the same object instance appears
             * in an object graph it should be sent by reference using an AMF0.
             * 
             * Use the reference type to reduce redundant information from being serialized
             * and infinite loops from cyclical references.
             * 
             * Marker: object-marker
             **/
            typedef struct
            {
                unsigned char* data;
                unsigned short int length;
            } Object;

            /**
             * Reference type.
             * 
             * AMF0 defines a complex object as an anonymous object, a typed object, an array
             * or an ecma-array. If the exact same instance of a complex object appears more 
             * than once in an object graph, then it must be sent by reference. The reference
             * type uses an unsigned 16-bit integer to an index in a table of previously 
             * serialized objects. Indices start at 0.
             **/
            typedef struct
            {
                unsigned short int reference;
            } Reference;

            /**
             * Long String Type
             * 
             * Marker: long-string-marker.
             * 
             * A long string type is used in AMF0 to encode strings that would
             * occupy more than 65535 bytes when UTF-8 encoded. The byte-length
             * header for the UTF-8 encoded string is a 32-bit integer instead
             * of a regular 16-bit integer.
             **/
            typedef struct
            {
                string value;
                unsigned int length;
                unsigned char* data;
            } LongString;
            
    };
    
    class AMF0Decoder
    {
        private:
            
            static int FindIndex(unsigned char* bytes, int size, AMF0::type_markers marker)
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

            static AMF0::Number DecodeNumber(unsigned char* bytes)
            {
                return AMF0::Number{
                    Utils::Math::IE754ToDouble(bytes),
                    bytes
                };
            }

            static AMF0::String DecodeString(unsigned char* bytes, unsigned short length)
            {
                char* strValue = new char[length];
                for (int i = 0; i < length; i++)
                    strValue[i] = bytes[i];
                return AMF0::String{
                    strValue,
                    length,
                    bytes
                };
            }

            static AMF0::Boolean DecodeBoolean(unsigned char* bytes)
            {
                return AMF0::Boolean{
                    (bool)((bytes[0] > 0) ? 1 : 0),
                    bytes
                };
            }

            static AMF0::Object DecodeObject(unsigned char* bytes)
            {
                return AMF0::Object{
                    bytes
                };
            }

            static AMF0::LongString DecodeLongString(unsigned char* bytes, unsigned int length)
            {
                return AMF0::LongString{
                    reinterpret_cast<char*>(bytes),
                    length,
                    bytes
                };
            }

            


        public:

            typedef struct
            {

            } ITEM;

            static void Decode(unsigned char* bytes, int size)
            {
                AMF0::type_markers lastMarker = (AMF0::type_markers)-1;
                int lastIndex = 0;
                bool doneWithItem = true;

                int itemCount = 0;

                while (lastIndex < size -1)
                {
                    // Looking for beginning marker
                    if (lastMarker == -1)
                    {
                        if (bytes[lastIndex] == AMF0::type_markers::string_marker)
                        {
                            // Found begin marker / string for command name.
                            lastMarker = AMF0::type_markers::string_marker;
                        }
                        else
                        {
                            // Error. Message should start with a command name (ie 0x02).
                            printf("\nSkipping index %i, string marker not found.", lastIndex);
                            lastIndex++;
                        }
                    }
                    else
                    {
                        int length;
                        int endIndex = -1;
                        unsigned char* data;
                        

                        // Type initialization
                        AMF0::Number number;
                        AMF0::Boolean boolean;
                        AMF0::String str;
                        AMF0::Object object;
                        AMF0::Reference reference;

                        
                        int nextItemMarkerIndex = lastIndex + 1;
                        if ((itemCount > 0) && bytes[nextItemMarkerIndex] < 18)
                        {
                            lastMarker = (AMF0::type_markers)bytes[nextItemMarkerIndex];
                        }
                        else
                        {
                            printf("\nError, next byte is not a marker. %X", bytes[nextItemMarkerIndex]);
                        }

                        switch (lastMarker)
                        {
                            case AMF0::type_markers::number_marker:
                                length = 8;
                                number =  DecodeNumber(Get(bytes, length, lastIndex + 1));
                                lastIndex += 1 + length;
                                printf("\nNumber value: %d", number.value);
                                break;

                            case AMF0::type_markers::boolean_marker:
                                printf("\nBoolean marker found. %i", lastIndex);
                                lastIndex = size;
                                break;

                            case AMF0::type_markers::string_marker:
                                // Find string length
                                BitOperations::bytesToInteger(
                                    length, 
                                    new unsigned char[2]{bytes[lastIndex + 1], bytes[lastIndex + 2]}, 
                                    false, 
                                    2);
                                data = Get(bytes, length, lastIndex + 3);
                                str = DecodeString(data, length);
                                lastIndex += length + 2;
                                printf("\nString value: %s, of length %i", str.value, length);
                                break;

                            case AMF0::type_markers::object_marker:
                                data = Get(bytes, (size - lastIndex), lastIndex + 1);
                                endIndex = FindIndex(data, (size - lastIndex + 1), AMF0::type_markers::object_end_marker);
                                data = Get(data, (endIndex - lastIndex + 1), lastIndex + 1);
                                length = endIndex - lastIndex;
                                lastIndex += endIndex + 1;
                                printf("\nObject size. %i", length);
                                break;

                            case AMF0::type_markers::reference_marker:
                                printf("\nReference marker found. %i", lastIndex);
                                lastIndex = size;
                                break;

                            default:
                                printf("\nUnsupported AMF0 type. Type %i", lastMarker);
                                itemCount--;
                                lastIndex = size;
                                break;
                        };
                        itemCount++;
                    }
                }
            }
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
                    FindIndex(bytes, size, AMF0::type_markers::string_marker);
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
                    if (bytes[transactionIDIndex] == AMF0::type_markers::number_marker)
                    {
                        transactionIDArray = Get(bytes, 8, transactionIDIndex + 1);
                        transactionID = Utils::Math::IE754ToDouble(transactionIDArray);

                        printf("\nTransaction ID: %i", transactionID);
                    }
                }

                printf("\n");
            };
    };

    
}