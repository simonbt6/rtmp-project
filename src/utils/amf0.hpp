/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-24
 **/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <iomanip>
#include <string>

#include "Bit.hpp"
#include "Math.hpp"
#include "FormatedPrint.hpp"
#include "../core/Netconnection.hpp"

using namespace std;

/**
 * Type defintions
 **/

#define __DEBUG true


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
                unsigned int length;
                Netconnection::Object object;
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

            typedef struct
            {
                Number* numbers;
                unsigned short numberCount = 0;

                Boolean* booleans;
                unsigned short booleanCount = 0;

                String* strings;
                unsigned short stringCount = 0;

                Object* objects;
                unsigned short objectCount = 0;

                Reference* references;
                unsigned short referenceCount = 0;

            } Message;
            
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

            template <typename IntegerType = int>
            static void DecodeNumber(unsigned char* bytes, int size, int& index, IntegerType& value)
            {
                int length = 8;
                unsigned char* data = Get(bytes, length, index + 2);
                index += 1 + length;
                value = Utils::Math::IE754ToDouble(data);
            }

            static void DecodeString(unsigned char* bytes, int size, int& index, string& value)
            {
                // Find string length
                int length = 0;
                BitOperations::bytesToInteger(
                    length, 
                    new unsigned char[2]{bytes[index + 1], bytes[index + 2]}, 
                    false, 
                    2);
                unsigned char* data = Get(bytes, length, index + 3);
                index += length + 2;
                value = reinterpret_cast<char*>(data);
            }

            static void DecodeBoolean(unsigned char* bytes, int size, int& index, bool& value)
            {
                
            }

            static void DecodeObject(unsigned char* bytes, int size, int& index, Netconnection::Object& object)
            {
                unsigned char* data = Get(bytes, (size - index), index + 1);
                int endIndex = FindIndex(data, (size - index + 1), AMF0::type_markers::object_end_marker);
                int length = endIndex - index;
                data = Get(data, (length + 1), index + 1);
                index += endIndex + 1;
            }

            static void DecodeLongString(unsigned char* bytes, int& index, string& value)
            {
                
            }

            static CommandType FindCommandType(string commandName)
            {
                try
                {
                    return Netconnection::CommandLinker.at(commandName);
                }
                catch(std::out_of_range& e)
                {
                    std::cerr << "\nOut of range error: " << e.what() << std::endl;
                }

                return CommandType::Null;
            }

        public:

            static Netconnection::Command* DecodeCommand(unsigned char* bytes, int size)
            {
                unsigned char* data;
                /**
                 * Find first item marker (string).
                 * Parse item
                 * Determine command type from command name.
                 * Find second item (last item index + 1)
                 * Parse
                 * Repeat
                 **/

                /**
                 * Find first item marker.
                 **/
                int commandNameIndex = FindIndex(bytes, size, AMF0::type_markers::string_marker);
                int commandNameLength = 0;
                BitOperations::bytesToInteger(
                    commandNameLength, 
                    new unsigned char[2]{bytes[commandNameIndex + 1], bytes[commandNameIndex + 2]}, 
                    false, 
                    2);
                data = Get(bytes, commandNameLength, commandNameIndex + 3);
                string commandNameString;
                DecodeString(data, size, commandNameLength, commandNameString);
                printf("\nCommand name: %s\n", commandNameString);

                /**
                 * Find command type.
                 **/
                CommandType commandType = CommandType::Null();
                commandType = Netconnection::CommandLinker[commandNameString];

                /**
                 * Parse appropriate informations according to command type.
                 **/
                int lastIndex = commandNameIndex;
                switch (commandType)
                {
                    case CommandType::Connect:
                    {
                        Netconnection::Connect command;
                        /**
                         * 1. Transaction ID.
                         * 2. Command object: Object.
                         * 3. Optional user arguments: Object.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Command object.
                         **/
                        DecodeObject(bytes, size, lastIndex, command.CommandObject);

                        /**
                         * Checks if an optional user argument object is present.
                         **/
                        if (lastIndex >= size) return &command;
                        
                        /**
                         * Optional user arguments.
                         **/
                        DecodeObject(bytes, size, lastIndex, command.OptionalUserArguments);

                        return &command;
                        break;
                    }

                    case CommandType::ConnectResponse:
                    {
                        Netconnection::ConnectResponse command;
                        /**
                         * 1. Transaction ID. Should be 1.
                         * 2. Properties: Object.
                         * 3. Information: Object.
                         **/
                        
                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Properties.
                         **/
                        DecodeObject(bytes, size, lastIndex, command.Properties);

                        /**
                         * Information.
                         **/
                        DecodeObject(bytes, size, lastIndex, command.Information);

                        return &command;
                        break;
                    }

                    case CommandType::Call:
                    {
                        Netconnection::Call command;
                        /**
                         * 1. Procedure/Command Name: String
                         * 2. Transaction ID: Number.
                         * 3. Command object: Object.
                         * 4. Optional arguments.
                         **/

                        /**
                         * Procedure name.
                         **/
                        DecodeString(bytes, size, lastIndex, command.CommandName);

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Command object.
                         **/
                        DecodeObject(bytes, size, lastIndex, command.CommandObject);

                        /**
                         * Optional arguments.
                         **/
                        DecodeObject(bytes, size, lastIndex, command.OptionalArguments);
                        
                        return &command;
                        break;
                    }

                    case CommandType::CallResponse:
                    {
                        Netconnection::CallResponse command;
                        /**
                         * 1. Transaction ID: Number. 
                         *    - ID of the command, to which the response belongs.
                         * 2. Command object: Object.
                         * 3. Response: Object.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Command object.
                         **/
                        DecodeObject(bytes, size, lastIndex, command.CommandObject);

                        /**
                         * Response.
                         **/
                        DecodeObject(bytes, size, lastIndex, command.Response);

                        return &command;
                        break;
                    }    
                    
                    case CommandType::CreateStream:
                    {
                        Netconnection::CreateStream command;
                        /**
                         * 1. Transaction ID: Number.
                         * 2. Command object: Object.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Command object.
                         **/
                        DecodeObject(bytes, size, lastIndex, command.CommandObject);

                        return &command;
                        break;
                    }

                    case CommandType::CreateStreamResponse:
                    {
                        Netconnection::CreateStreamResponse command;
                        /**
                         * 1. Command Name: String. _result or _error.
                         * 2. Transaction ID: Number. 
                         *    - ID of the command , to which the response belongs.
                         * 3. Stream ID: Number.
                         **/
                        
                        /**
                         * Command name.
                         **/
                        DecodeString(bytes, size, lastIndex, command.CommandName);

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Stream ID.
                         **/
                        DecodeNumber<unsigned int>(bytes, size, lastIndex, command.StreamID);

                        return &command;
                        break;
                    }

                    case CommandType::OnStatus:
                    {
                        Netconnection::OnStatus command;
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Info object: Object.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Information.
                         **/
                        DecodeObject(bytes, size, lastIndex, command.Information);

                        return &command;
                        break;
                    }

                    case CommandType::Play:
                    {
                        Netconnection::Play command;
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Stream name: String. 
                         * 4. Start: Number.
                         * 5. Duration: Number.
                         * 6. Reset: Bool.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Stream name.
                         **/
                        DecodeString(bytes, size, lastIndex, command.StreamName);

                        /**
                         * Start.
                         **/
                        DecodeNumber<int>(bytes, size, lastIndex, command.Start);

                        /**
                         * Duration.
                         **/
                        DecodeNumber<int>(bytes, size, lastIndex, command.duration);

                        /**
                         * Reset.
                         **/
                        DecodeBoolean(bytes, size, lastIndex, command.Reset);

                        return &command;
                        break;
                    }

                    case CommandType::Play2:
                    {
                        Netconnection::Play2 command;
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Parameters: Object.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Parameters.
                         **/
                        DecodeObject(bytes, size, lastIndex, command.Parameters);

                        return &command;
                        break;
                    }

                    case CommandType::DeleteStream:
                    {
                        Netconnection::DeleteStream command;
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Stream ID: Number.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Stream ID.
                         **/
                        DecodeNumber(bytes, size, lastIndex, command.StreamID);

                        return &command;
                        break;
                    }
                    case CommandType::ReceiveAudio:
                    {
                        Netconnection::ReceiveAudio command;
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Bool flag: Bool.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Bool flag.
                         **/
                        DecodeBoolean(bytes, size, lastIndex, command.BoolFlag);

                        return &command;
                        break;
                    }

                    case CommandType::ReceiveVideo:
                    {
                        Netconnection::ReceiveVideo command;
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Bool flag: Bool.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Bool flag.
                         **/
                        DecodeBoolean(bytes, size, lastIndex, command.BoolFlag);

                        return &command;
                        break;
                    }

                    case CommandType::Publish:
                    {
                        Netconnection::Publish command;
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Publishing name: String.
                         * 4. Publishing type: String.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Publishing name.
                         **/
                        DecodeString(bytes, size, lastIndex, command.PublishingName);

                        /**
                         * Publishing type.
                         **/
                        DecodeString(bytes, size, lastIndex, command.PublishingType);

                        return &command;
                        break;
                    }

                    case CommandType::Seek:
                    {
                        Netconnection::Seek command;
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Milliseconds: Number.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Milliseconds.
                         **/
                        DecodeNumber<int>(bytes, size, lastIndex, command.milliseconds);
                        
                        return &command;
                        break;
                    }

                    case CommandType::Pause:
                    {
                        Netconnection::Pause command;
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Pause: Bool.
                         * 4. Milliseconds: Number.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, command.TransactionID);

                        /**
                         * Pause.
                         **/
                        DecodeBoolean(bytes, size, lastIndex, command.Pause);

                        /**
                         * Milliseconds.
                         **/
                        DecodeNumber<int>(bytes, size, lastIndex, command.Milliseconds);
                        
                        return &command;
                        break;
                    }

                    case CommandType::Null:
                        printf("Error, null command type.");
                        break;
                };


            }
            

            /**
             * TODO: Convert this to parse Object (map<string, Property*>).
             **/
            static AMF0::Message Decode(unsigned char* bytes, int size)
            {
                AMF0::type_markers lastMarker = (AMF0::type_markers)-1;
                int lastIndex = 0;
                bool doneWithItem = true;
                bool firstElement = false;

                int itemCount = 0;

                AMF0::Message message;
                        
                vector<AMF0::Number> numbers;
                vector<AMF0::Boolean> booleans;
                vector<AMF0::String> strings;
                vector<AMF0::Object> objects;
                vector<AMF0::Reference> references;

                Netconnection::Command* command;
                CommandType commandType = CommandType::Null;



                

                while (lastIndex < size -1)
                {
                    // Looking for beginning marker
                    if (lastMarker == -1)
                    {
                        if (bytes[lastIndex] == AMF0::type_markers::string_marker)
                        {
                            // Found begin marker / string for command name.
                            lastMarker = AMF0::type_markers::string_marker;
                            firstElement = true;
                        }
                        else
                        {
                            // Error. Message should start with a command name (ie 0x02).
                            #if __DEBUG
                                printf("\nSkipping index %i, string marker not found.", lastIndex);
                            #endif
                            lastIndex++;
                        }
                    }
                    else
                    {
                        int length;
                        int endIndex = -1;
                        unsigned char* data;

                        string commandName;


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
                            #if __DEBUG
                                printf("\nError, next byte is not a marker. %X", bytes[nextItemMarkerIndex]);
                            #endif
                        }

                        switch (lastMarker)
                        {
                            case AMF0::type_markers::number_marker:
                                
                                break;

                            case AMF0::type_markers::boolean_marker:
                                printf("\nBoolean marker found. %i", lastIndex);
                                lastIndex = size;
                                break;

                            case AMF0::type_markers::string_marker:
                                
                                break;

                            case AMF0::type_markers::object_marker:
                                
                                break;

                            case AMF0::type_markers::reference_marker:
                                printf("\nReference marker found. %i", lastIndex);
                                lastIndex = size;
                                // references.push_back(reference);
                                break;
                            
                            case AMF0::null_marker:
                                // Ignores  null markers for now.
                                lastIndex++;
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
                return message;
            }
    };

    
}