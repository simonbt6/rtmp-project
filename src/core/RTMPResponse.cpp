#include "RTMPResponse.hpp"

namespace RTMP
{
    vector<char> ServerResponse::ConnectResponse(Session& session)
    {
        vector<char> data;
        
        Netconnection::Connect* command = dynamic_cast<Netconnection::Connect*>(session.pendingCommand);
        if (command == NULL)
        {
            Utils::FormatedPrint::PrintError("ServerResponse::ConnectResponse", "Netconnection::Connect command cast failed.");
            return data;
        }
        bool success = true;
        string commandName = success ? "_result" : "_error";
        int transactionID = 1;
        

        Utils::AMF0::Data commandNameData =
            Utils::AMF0Encoder::EncodeString(commandName);

        Utils::AMF0::Data transactionIDData = 
            Utils::AMF0Encoder::EncodeNumber(transactionID);

        /**
         * Properties object.
         */
        Utils::AMF0::Data propertiesData = 
            Utils::AMF0Encoder::EncodeObject(command->CommandObject);

        data.insert(data.end(), commandNameData.data, commandNameData.data + commandNameData.size);
        data.insert(data.end(), transactionIDData.data, transactionIDData.data + transactionIDData.size);
        // data.insert(data.end(), propertiesData.data, propertiesData.data + propertiesData.size);

        return data;
        
    }

    vector<char> ServerResponse::CallResponse(Session& session)
    {
        vector<char> data;

        return data;
    }

    vector<char> ServerResponse::CreateStreamResponse(Session& session)
    {
        vector<char> data;

        return data;
    }
}