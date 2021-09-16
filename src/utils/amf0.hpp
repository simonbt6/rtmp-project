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
#include <map>

#include "Bit.hpp"
#include "Math.hpp"
#include "FormatedPrint.hpp"
#include "Object.hpp"

#include "../core/Netconnection.hpp"

using namespace std;

#define __DEBUG true


namespace Utils
{
    class AMF0
    {
        public:

            /**
             * Used to convay data.
             **/
            typedef struct
            {
                unsigned char* data;
                int size;
            } Data;

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

    };

    class AMF0Encoder
    {
        public:
            static AMF0::Data EncodeNumber(double value)
            {
                AMF0::Data data;

                data.size = 9;
                data.data = new unsigned char[data.size];

                data.data[0] = AMF0::type_markers::number_marker;

                int* numberData = Utils::Math::DoubleToIEEE754(value);
                for (int i = 0; i < data.size - 1; i++)
                    data.data[i + 1] = numberData[i];

                return data;
            }

            static AMF0::Data EncodeBoolean(bool value)
            {
                AMF0::Data data;

                data.size = 2;
                data.data = new unsigned char[data.size];
                
                data.data[0] = AMF0::type_markers::boolean_marker;
                data.data[1] = value;

                return data;
            }

            static AMF0::Data EncodeString(string& value)
            {
                AMF0::Data data;
                data.size = value.length() + 3;
                data.data = new unsigned char[data.size];

                data.data[0] = AMF0::type_markers::string_marker;
                
                data.data[1] = value.length() / 256;
                data.data[2] = value.length() % 256;

                for (int i = 0; i < value.length(); i++)
                    data.data[i + 3] = value[i];

                return data;
            }

            static AMF0::Data EncodeObject(Object& value)
            {
                AMF0::Data data;
                vector<unsigned char> vData;

                // Object marker.
                vData.insert(vData.end(), 1, 0x03);

                for (pair p : value)
                {
                    /**
                     * Property Name Data.  
                     */
                    std::string propertyNameString = 
                        PropertyNameLinker.at(p.first);
                    Utils::FormatedPrint::PrintFormated(
                        "AMF0Encoder::EncodeObject",
                        "Key name: " + propertyNameString
                    );
                    vData.insert(vData.end(), propertyNameString.data(), propertyNameString.data() + propertyNameString.length());
                    
                    /**
                     * Property Data.
                     */
                    Property* property = p.second;
                    if (property == NULL) 
                    {
                        Utils::FormatedPrint::PrintFormated(
                            "AMF0Encoder::EncodeObject", 
                            "Error, cannot cast field type.");
                        break;
                    }
                    if (Field<double>* field = dynamic_cast<Field<double>*>(property))
                    {
                        double fieldValue = field->value;

                        AMF0::Data fieldData = EncodeNumber(fieldValue);
                        vData.insert(vData.end(), fieldData.data, fieldData.data + fieldData.size);
                    }
                    else if (Field<bool>* field = dynamic_cast<Field<bool>*>(property))
                    {
                        bool fieldValue = field->value;
                        
                        AMF0::Data fieldData = EncodeBoolean(fieldValue);
                        vData.insert(vData.end(), fieldData.data, fieldData.data + fieldData.size);
                    }
                    else if (Field<string>* field = dynamic_cast<Field<string>*>(property))
                    {
                        string fieldValue = field->value;
                        
                        AMF0::Data fieldData = EncodeString(fieldValue);
                        vData.insert(vData.end(), fieldData.data, fieldData.data + fieldData.size);

                        Utils::FormatedPrint::PrintFormated(
                            "AMF0Encoder::EncodeObject", 
                            "Field value: " + fieldValue + ".");
                    }
                    else if (Field<Object>* field = dynamic_cast<Field<Object>*>(property))
                    {
                        Object fieldValue = field->value;
                        Utils::FormatedPrint::PrintFormated(
                            "AMF0Encoder::EncodeObject",
                            "Object datatype."
                        );

                        AMF0::Data fieldData = EncodeObject(fieldValue);
                        vData.insert(vData.end(), fieldData.data, fieldData.data + fieldData.size);
                    }                 
                }


                // End of object marker.
                unsigned char endMarker[3]{0, 0, 0x09};
                vData.insert(vData.end(), endMarker, endMarker + 3);

                data.size = vData.size();
                data.data = new unsigned char[data.size];

                for (int i = 0; i < data.size; i++)
                    data.data[i] = vData.at(i);

                return data;
            }
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
                unsigned char* data = Get(bytes, length, index + 1);
                index += 1 + length;
                value = Utils::Math::IE754ToDouble(data);
                Utils::FormatedPrint::PrintFormated(
                    "AMF0Decoder::DecodeNumber", 
                    "Number value: " + to_string(value) + ".");
            }

            static void DecodeString(unsigned char* bytes, int size, int& index, string& value)
            {
                printf("\n");

                if (size == 0) 
                    Utils::FormatedPrint::PrintError(
                        "AMF0Decoder::DecodeString", 
                        "Error, data size is 0.");

                int length = 0;

                Utils::FormatedPrint::PrintFormated(
                    "AMF0Decoder::DecodeString", 
                    "Index: " + to_string(index) + ".");

                Utils::FormatedPrint::PrintFormated(
                    "AMF0Decoder::DecodeString", 
                    "String length bytes: " + to_string(bytes[index + 1]) + ", " + to_string(bytes[index + 2]) + ".");
                
                BitOperations::bytesToInteger(
                    length, 
                    new unsigned char[2]{bytes[index + 1], bytes[index + 2]}, 
                    false, 
                    2);

                if ((index + length) > size)
                {
                    Utils::FormatedPrint::PrintError(
                        "AMF0Decoder::DecodeString",
                        "Error, String out of range.");
                    return;
                }

                Utils::FormatedPrint::PrintFormated(
                    "AMF0Decoder::DecodeString", 
                    "Calculated string length: " + to_string(length) + ".");
             
                unsigned char* data = Get(bytes, length, index + 3);
                index += length + 3;
                value = reinterpret_cast<char*>(data);

                // Removes parasite data.
                value = value.substr(0, length);
                Utils::FormatedPrint::PrintFormated(
                    "AMF0Decoder::DecodeString", 
                    "Actual string length: " + to_string((signed)value.length()) + ".");
                Utils::FormatedPrint::PrintFormated(
                    "AMF0Decoder::DecodeString",
                    "String value: " + value
                );
            }

            static void DecodeBoolean(unsigned char* bytes, int size, int& index, bool& value)
            {
                
            }

            static void DecodeObject(unsigned char* bytes, int size, int& index, Object& object)
            {
                Utils::FormatedPrint::PrintBytes<unsigned char>(bytes, size);
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

            static Netconnection::CommandType FindCommandType(string commandName)
            {
                try
                {
                    return Netconnection::CommandLinker.at(commandName);
                }
                catch(std::out_of_range& e)
                {
                    Utils::FormatedPrint::PrintError(
                        "AMF0Decoder::FindCommandType", 
                        "Out of range error: " + string(e.what()) + ".");
                }

                return Netconnection::CommandType::Null;
            }

            static Property* DecodeField(unsigned char* bytes, int size, int& index, AMF0::type_markers lastMarker)
            {
                unsigned char* data;
                switch (lastMarker)
                    {
                        case AMF0::type_markers::number_marker:       
                        {
                            Field<int>* field = new Field<int>();
                            
                            DecodeNumber(bytes, size, index, field->value);

                            return field;
                            break;
                        }

                        case AMF0::type_markers::boolean_marker:
                        {
                            Field<bool>* field = new Field<bool>();
                            DecodeBoolean(bytes, size, index, field->value);

                            return field;
                            break;
                        }

                        case AMF0::type_markers::string_marker:
                        {
                            Field<string>* field = new Field<string>();
                            DecodeString(bytes, size, index, field->value);

                            return field;
                            break;
                        }

                        case AMF0::type_markers::object_marker:
                        {
                            Field<Object>* field = new Field<Object>();
                            DecodeObject(bytes, size, index, field->value);
                            FormatedPrint::PrintObject(field->value);

                            return field;
                            break;
                        }

                        case AMF0::type_markers::reference_marker:
                        {
                            Utils::FormatedPrint::PrintFormated(
                                "AMF0Decoder::DecodeString", 
                                "Error, unsupported type marker. Reference marker.");
                            return nullptr;
                            break;
                        }
                        
                        case AMF0::null_marker:
                        {
                            Utils::FormatedPrint::PrintFormated(
                                "AMF0Decoder::DecodeString", 
                                "Error, unsupported type marker. Null marker.");
                            index++;
                            return nullptr;
                            break;
                        }

                        default:
                            Utils::FormatedPrint::PrintFormated(
                                "AMF0Decoder::DecodeString", 
                                "Error, unsupported type marker. Type " + to_string(lastMarker)+ ".");
                            index = size; // Kills the process.
                            return nullptr;
                            break;
                    };
                    return nullptr;
            }


            static Object DecodeObjectProperties(unsigned char* bytes, int size)
            {
                AMF0::type_markers lastMarker = (AMF0::type_markers)-1;
                int lastIndex = 0;
                bool firstElement = false;

                Object object;    

                Utils::FormatedPrint::PrintBytes<unsigned char>(bytes, size);            

                while (lastIndex < size -1)
                {
                    if (lastIndex > 0) lastIndex--;
                    
                    int length;
                    unsigned char* data;

                    string key;
                    DecodeString(bytes, size, lastIndex, key);

                    Utils::FormatedPrint::PrintFormated(
                        "AMF0Decoder::DecodeObjectProperties", 
                        "Key string: " + key + ".");

                    lastMarker = (AMF0::type_markers)bytes[lastIndex];
                    if (((int)lastMarker) > 0x10) break;
                    Property* property = DecodeField(bytes, size, lastIndex, lastMarker);
                    PropertyType propertyType = PropertyTypeLinker.at(key);

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
                Utils::FormatedPrint::PrintFormated(
                    "AMF0Decoder::DecodeCommand", 
                    "Command name: " + commandNameString + ".");

                /**
                 * Find command type.
                 **/
                Netconnection::CommandType commandType = Netconnection::CommandType::Null;
                commandType = Netconnection::CommandLinker[commandNameString];
                Utils::FormatedPrint::PrintFormated(
                    "AMF0Decoder::DecodeCommand", 
                    "Command type: " + to_string((int)commandType) + ".");

                /**
                 * Parse appropriate informations according to command type.
                 **/
                Utils::FormatedPrint::PrintFormated(
                    "AMF0Decoder::DecodeCommand", 
                    "Command body: ");
                Utils::FormatedPrint::PrintBytes<unsigned char>(bytes, size);
                switch (commandType)
                {
                    case Netconnection::CommandType::Connect:
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
                        Utils::FormatedPrint::PrintFormated(
                            "AMF0Decoder::DecodeCommand", 
                            "Transaction ID: " + to_string(cmd->TransactionID) + ".");

                        /**
                         * Command object.
                         **/
                        Utils::FormatedPrint::PrintBytes<unsigned char>(bytes, size);
                        unsigned char* commandObjectData = Get(bytes, (size - lastIndex), lastIndex);
                        cmd->CommandObject = DecodeObjectProperties(commandObjectData, (size - lastIndex));
                        

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
                        //cmd->OptionalUserArguments = DecodeObjectProperties(bytes, size, lastIndex, );
                                             
                        return cmd;  
                        break;
                    }

                    case Netconnection::CommandType::ConnectResponse:
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

                    case Netconnection::CommandType::Call:
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

                    case Netconnection::CommandType::CallResponse:
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
                    
                    case Netconnection::CommandType::CreateStream:
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
                        Utils::FormatedPrint::PrintFormated(
                            "AMF0Decoder::DecodeCommand", 
                            "Transaction ID: " + to_string(cmd->TransactionID) + ".");

                        /**
                         * Command object.
                         **/
                        //DecodeObject(bytes, size, lastIndex, cmd->CommandObject);

                        /**
                         * Null
                         **/
                        lastIndex++;

                        return cmd;
                        break;
                    }

                    case Netconnection::CommandType::CreateStreamResponse:
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

                    case Netconnection::CommandType::OnStatus:
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

                    case Netconnection::CommandType::Play:
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

                    case Netconnection::CommandType::Play2:
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

                    case Netconnection::CommandType::DeleteStream:
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
                    case Netconnection::CommandType::ReceiveAudio:
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

                    case Netconnection::CommandType::ReceiveVideo:
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

                    case Netconnection::CommandType::Publish:
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
                        Utils::FormatedPrint::PrintFormated(
                            "AMF0Decoder::DecodeCommand", 
                            "Transaction ID: " + to_string(cmd->TransactionID) + ".");

                        /**
                         * Null object.
                         **/
                        lastIndex++;

                        /**
                         * Publishing name.
                         **/
                        DecodeString(bytes, size, lastIndex, cmd->PublishingName);
                        Utils::FormatedPrint::PrintFormated(
                            "AMF0Decoder::DecodeCommand", 
                            "Publishing name: " + cmd->PublishingName + ".");

                        /**
                         * Publishing type.
                         **/
                        DecodeString(bytes, size, lastIndex, cmd->PublishingType);
                        Utils::FormatedPrint::PrintFormated(
                            "AMF0Decoder::DecodeCommand", 
                            "Publishing name: " + cmd->PublishingType + ".");

                        return cmd;
                        break;
                    }

                    case Netconnection::CommandType::Seek:
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

                    case Netconnection::CommandType::Pause:
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

                    case Netconnection::CommandType::Null:
                        Utils::FormatedPrint::PrintFormated(
                            "AMF0Decoder::DecodeCommand", 
                            "Error, null command type.");
                        break;
                };
                return nullptr;
            }
    };

    
}
