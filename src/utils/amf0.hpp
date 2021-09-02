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
                if (size == 0) printf("\nError, data size is 0.");
                int length = 0;
                printf("\nIndex: %i", index);
                printf("\nString length bytes: %X, %X", bytes[index + 1], bytes[index + 2]);
                
                BitOperations::bytesToInteger(
                    length, 
                    new unsigned char[2]{bytes[index + 1], bytes[index + 2]}, 
                    false, 
                    2);
             
                unsigned char* data = Get(bytes, length, index + 3);
                printf("\nData length: %i", length);
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
                // TODO: Implement long string (> 65535 bytes) decoding.
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

            static Property* DecodeField(unsigned char* bytes, int size, int& index, AMF0::type_markers lastMarker)
            {
                unsigned char* data;
                switch (lastMarker)
                    {
                        case AMF0::type_markers::number_marker:       
                        {
                            Field<int>* field = new Field<int>();
                            
                            DecodeNumber(bytes, size, index, field->m_Value);

                            return field;
                            break;
                        }

                        case AMF0::type_markers::boolean_marker:
                        {
                            Field<bool>* field = new Field<bool>();
                            DecodeBoolean(bytes, size, index, field->m_Value);

                            return field;
                            break;
                        }

                        case AMF0::type_markers::string_marker:
                        {
                            Field<string>* field = new Field<string>();
                            DecodeString(bytes, size, index, field->m_Value);

                            return field;
                            break;
                        }

                        case AMF0::type_markers::object_marker:
                        {
                            Field<Netconnection::Object>* field = new Field<Netconnection::Object>();
                            DecodeObject(bytes, size, index, field->m_Value);

                            return field;
                            break;
                        }

                        case AMF0::type_markers::reference_marker:
                        {
                            printf("\nError, unsupported type marker. Reference marker.");
                            return nullptr;
                            break;
                        }
                        
                        case AMF0::null_marker:
                        {
                            printf("\nError, unsupported type marker. Null marker.");
                            index++;
                            return nullptr;
                            break;
                        }

                        default:
                            printf("\nUnsupported AMF0 type. Type %i", lastMarker);
                            index = size; // Kills the process.
                            return nullptr;
                            break;
                    };
                    return nullptr;
            }


            static Netconnection::Object DecodeObjectProperties(unsigned char* bytes, int size)
            {
                AMF0::type_markers lastMarker = (AMF0::type_markers)-1;
                int lastIndex = 0;
                bool firstElement = false;

                Netconnection::Object object;                

                while (lastIndex < size -1)
                {
                    
                    int length;
                    unsigned char* data;

                    string key;
                    DecodeString(bytes, size, lastIndex, key);
                    lastMarker = (AMF0::type_markers)bytes[lastIndex + 1];
                    Property* property = DecodeField(bytes, size, lastIndex, lastMarker);
                    PropertyType propertyType = Netconnection::propertyTypeLinker[key];

                    if (property != nullptr)
                        object.insert(pair<PropertyType, Property*> (propertyType, property));

                }
                return object;
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

                int lastIndex = 0;
                /**
                 * Find first item marker.
                 **/
                lastIndex = FindIndex(bytes, size, AMF0::type_markers::string_marker);
                string commandNameString;
                DecodeString(bytes, size, lastIndex, commandNameString);
                printf("\nCommand name: %s", commandNameString.c_str());

                /**
                 * Find command type.
                 **/
                CommandType commandType = CommandType::Null;
                commandType = Netconnection::CommandLinker[commandNameString];
                printf("\nCommand type: %i\n", commandType);

                /**
                 * Parse appropriate informations according to command type.
                 **/
                printf("\nCommand body: ");
                switch (commandType)
                {
                    case CommandType::Connect:
                    {
                        Netconnection::Connect* cmd = new Netconnection::Connect();
                        /**
                         * 1. Transaction ID.
                         * 2. Command object: Object.
                         * 3. Optional user arguments: Object.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);
                        printf("\nTransaction ID: %i", cmd->TransactionID);

                        /**
                         * Command object.
                         **/
                        //DecodeObject(bytes, size, lastIndex, command.CommandObject);
                        

                        /**
                         * Checks if an optional user argument object is present.
                         **/
                        if (lastIndex >= size) {
                            return cmd;
                            break;
                        }
                        
                        /**
                         * Optional user arguments.
                         **/
                        //DecodeObject(bytes, size, lastIndex, command.OptionalUserArguments);
                        return cmd;  
                        printf("­\nBreak.");                      
                        break;
                    }

                    case CommandType::ConnectResponse:
                    {
                        Netconnection::ConnectResponse* cmd = new Netconnection::ConnectResponse();
                        /**
                         * 1. Transaction ID. Should be 1.
                         * 2. Properties: Object.
                         * 3. Information: Object.
                         **/
                        
                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Properties.
                         **/
                        DecodeObject(bytes, size, lastIndex, cmd->Properties);

                        /**
                         * Information.
                         **/
                        DecodeObject(bytes, size, lastIndex, cmd->Information);

                        return cmd;
                        break;
                    }

                    case CommandType::Call:
                    {
                        Netconnection::Call* cmd = new Netconnection::Call();
                        /**
                         * 1. Procedure/Command Name: String
                         * 2. Transaction ID: Number.
                         * 3. Command object: Object.
                         * 4. Optional arguments.
                         **/

                        /**
                         * Procedure name.
                         **/
                        DecodeString(bytes, size, lastIndex, cmd->CommandName);

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Command object.
                         **/
                        DecodeObject(bytes, size, lastIndex, cmd->CommandObject);

                        /**
                         * Optional arguments.
                         **/
                        DecodeObject(bytes, size, lastIndex, cmd->OptionalArguments);
                        
                        return cmd;
                        break;
                    }

                    case CommandType::CallResponse:
                    {
                        Netconnection::CallResponse* cmd = new Netconnection::CallResponse();
                        /**
                         * 1. Transaction ID: Number. 
                         *    - ID of the command, to which the response belongs.
                         * 2. Command object: Object.
                         * 3. Response: Object.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Command object.
                         **/
                        DecodeObject(bytes, size, lastIndex, cmd->CommandObject);

                        /**
                         * Response.
                         **/
                        DecodeObject(bytes, size, lastIndex, cmd->Response);

                        return cmd;
                        break;
                    }    
                    
                    case CommandType::CreateStream:
                    {
                        Netconnection::CreateStream* cmd = new Netconnection::CreateStream();
                        /**
                         * 1. Transaction ID: Number.
                         * 2. Command object: Object.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Command object.
                         **/
                        DecodeObject(bytes, size, lastIndex, cmd->CommandObject);

                        return cmd;
                        break;
                    }

                    case CommandType::CreateStreamResponse:
                    {
                        Netconnection::CreateStreamResponse* cmd = new Netconnection::CreateStreamResponse();
                        /**
                         * 1. Command Name: String. _result or _error.
                         * 2. Transaction ID: Number. 
                         *    - ID of the command , to which the response belongs.
                         * 3. Stream ID: Number.
                         **/
                        
                        /**
                         * Command name.
                         **/
                        DecodeString(bytes, size, lastIndex, cmd->CommandName);

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Stream ID.
                         **/
                        DecodeNumber<unsigned int>(bytes, size, lastIndex, cmd->StreamID);

                        return cmd;
                        break;
                    }

                    case CommandType::OnStatus:
                    {
                        Netconnection::OnStatus* cmd = new Netconnection::OnStatus();
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Info object: Object.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Information.
                         **/
                        DecodeObject(bytes, size, lastIndex, cmd->Information);

                        return cmd;
                        break;
                    }

                    case CommandType::Play:
                    {
                        Netconnection::Play* cmd = new Netconnection::Play();
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
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Stream name.
                         **/
                        DecodeString(bytes, size, lastIndex, cmd->StreamName);

                        /**
                         * Start.
                         **/
                        DecodeNumber<int>(bytes, size, lastIndex, cmd->Start);

                        /**
                         * Duration.
                         **/
                        DecodeNumber<int>(bytes, size, lastIndex, cmd->duration);

                        /**
                         * Reset.
                         **/
                        DecodeBoolean(bytes, size, lastIndex, cmd->Reset);

                        return cmd;
                        break;
                    }

                    case CommandType::Play2:
                    {
                        Netconnection::Play2* cmd = new Netconnection::Play2();
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Parameters: Object.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Parameters.
                         **/
                        DecodeObject(bytes, size, lastIndex, cmd->Parameters);

                        return cmd;
                        break;
                    }

                    case CommandType::DeleteStream:
                    {
                        Netconnection::DeleteStream* cmd = new Netconnection::DeleteStream();
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Stream ID: Number.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Stream ID.
                         **/
                        DecodeNumber(bytes, size, lastIndex, cmd->StreamID);

                        return cmd;
                        break;
                    }
                    case CommandType::ReceiveAudio:
                    {
                        Netconnection::ReceiveAudio* cmd = new Netconnection::ReceiveAudio();
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Bool flag: Bool.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Bool flag.
                         **/
                        DecodeBoolean(bytes, size, lastIndex, cmd->BoolFlag);

                        return cmd;
                        break;
                    }

                    case CommandType::ReceiveVideo:
                    {
                        Netconnection::ReceiveVideo* cmd = new Netconnection::ReceiveVideo();
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Bool flag: Bool.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Bool flag.
                         **/
                        DecodeBoolean(bytes, size, lastIndex, cmd->BoolFlag);

                        return cmd;
                        break;
                    }

                    case CommandType::Publish:
                    {
                        Netconnection::Publish* cmd = new Netconnection::Publish();
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Publishing name: String.
                         * 4. Publishing type: String.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Publishing name.
                         **/
                        DecodeString(bytes, size, lastIndex, cmd->PublishingName);

                        /**
                         * Publishing type.
                         **/
                        DecodeString(bytes, size, lastIndex, cmd->PublishingType);

                        return cmd;
                        break;
                    }

                    case CommandType::Seek:
                    {
                        Netconnection::Seek* cmd = new Netconnection::Seek();
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Milliseconds: Number.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Milliseconds.
                         **/
                        DecodeNumber<int>(bytes, size, lastIndex, cmd->milliseconds);
                        
                        return cmd;
                        break;
                    }

                    case CommandType::Pause:
                    {
                        Netconnection::Pause* cmd = new Netconnection::Pause();
                        /**
                         * 1. Transaction ID: Number. Should be 0.
                         * 2. Command object: Object. Should be NULL.
                         * 3. Pause: Bool.
                         * 4. Milliseconds: Number.
                         **/

                        /**
                         * Transaction ID.
                         **/
                        DecodeNumber<unsigned short>(bytes, size, lastIndex, cmd->TransactionID);

                        /**
                         * Pause.
                         **/
                        DecodeBoolean(bytes, size, lastIndex, cmd->pause);

                        /**
                         * Milliseconds.
                         **/
                        DecodeNumber<int>(bytes, size, lastIndex, cmd->Milliseconds);
                        
                        return cmd;
                        break;
                    }

                    case CommandType::Null:
                        printf("Error, null command type.");
                        break;
                };
                return nullptr;
            }
    };

    
}
