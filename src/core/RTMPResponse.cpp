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
            Utils::AMF0Encoder::EncodeString(commandName, true);

        Utils::AMF0::Data transactionIDData = 
            Utils::AMF0Encoder::EncodeNumber(transactionID);

        /**
         * Properties object.
         */
        Utils::Object propertiesObject;
        
        Utils::Field<string> fmsVerField;
        fmsVerField.value = "FMS/3,0,1,123";

        Utils::Field<double> capabilitiesField;
        capabilitiesField.value = 31;

        pair<Utils::PropertyType, Utils::Field<string>*> fmsPair {Utils::PropertyType::fmsVer, &fmsVerField};
        pair<Utils::PropertyType, Utils::Field<double>*> capabilitiesPair {Utils::PropertyType::capabilities, &capabilitiesField};
        propertiesObject.insert(fmsPair);
        propertiesObject.insert(capabilitiesPair);

        Utils::AMF0::Data propertiesData = 
            Utils::AMF0Encoder::EncodeObject(propertiesObject);

        for (int i = 0; i < propertiesData.size; i++)
            printf("\n->%X", propertiesData.data[i]);

        data.insert(data.end(), commandNameData.data, commandNameData.data + commandNameData.size);
        data.insert(data.end(), transactionIDData.data, transactionIDData.data + transactionIDData.size);
        data.insert(data.end(), propertiesData.data, propertiesData.data + propertiesData.size);

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
        
        Netconnection::CreateStream* command = dynamic_cast<Netconnection::CreateStream*>(session.pendingCommand);
        if (command == NULL)
        {
            Utils::FormatedPrint::PrintError("ServerResponse::ConnectResponse", "Netconnection::Connect command cast failed.");
            return data;
        }

        bool success = true;

        string commandName = success ? "_result" : "_error";
        
        session.streamID = 10;
        session.lastChunk->basicHeader.fmt = 0;
        session.lastChunk->messageHeader.message_stream_id = 0;

        Utils::Object commandObject;
        commandObject.insert(pair<Utils::PropertyType, Utils::Property*>(Utils::PropertyType::app, new Utils::Field<string>("test")));

        Utils::AMF0::Data commandNameData =
            Utils::AMF0Encoder::EncodeString(commandName, true);

        Utils::AMF0::Data transactionIDData = 
            Utils::AMF0Encoder::EncodeNumber(command->TransactionID);

        Utils::AMF0::Data streamIDData = 
            Utils::AMF0Encoder::EncodeNumber(session.streamID);

        Utils::AMF0::Data commandObjectData = 
            Utils::AMF0Encoder::EncodeObject(commandObject);
        Utils::FormatedPrint::PrintBytes<unsigned char>(commandObjectData.data, commandObjectData.size);

        data.insert(data.end(), commandNameData.data, commandNameData.data + commandNameData.size);
        data.insert(data.end(), transactionIDData.data, transactionIDData.data + transactionIDData.size);
        data.insert(data.end(), commandObjectData.data, commandObjectData.data + commandObjectData.size);
        data.insert(data.end(), streamIDData.data, streamIDData.data + streamIDData.size);

        return data;
    }

    vector<char> ServerResponse::OnStatus(Session& session, int level, string code, string description)
    {
        vector<char> data;

        Utils::Object commandObject;
        Utils::AMF0::Data amfdata;

        session.lastChunk->basicHeader.fmt = 0;

        string commandNameString = "onStatus";

        // Command name.
        amfdata = Utils::AMF0Encoder::EncodeString(commandNameString, true);
        data.insert(data.end(), amfdata.data, amfdata.data + amfdata.size);

        // Transaction ID.
        amfdata = Utils::AMF0Encoder::EncodeNumber(0.0);
        data.insert(data.end(), amfdata.data, amfdata.data + amfdata.size);

        // Null marker.
        data.insert(data.end(), 1, Utils::AMF0::type_markers::null_marker);

        // Level field
        commandObject.insert(pair<Utils::PropertyType, Utils::Property*>(Utils::PropertyType::level, new Utils::Field<string>(level ? "warning" : "status")));

        // Code field
        commandObject.insert(pair<Utils::PropertyType, Utils::Property*>(Utils::PropertyType::code, new Utils::Field<string>(code)));

        // Description field.
        if (description != "")
            commandObject.insert(pair<Utils::PropertyType, Utils::Property*>(Utils::PropertyType::description, new Utils::Field<string>(description)));
        
        // Command object.
        amfdata = Utils::AMF0Encoder::EncodeObject(commandObject);
        data.insert(data.end(), amfdata.data, amfdata.data + amfdata.size);

        return data;
    }

    vector<char> ServerResponse::StreamBegin(Session& session)
    {
        vector<char> data;

        /**
         * EventType -> 2 bytes.
         * EventData -> 4 bytes.
         */
        
        UserControlMessage::EventType eventType = 
            UserControlMessage::EventType::StreamBegin;
        session.streamID = 10;
        char eventData[4] = {
            session.streamID >> 24,
            (session.streamID >> 16) & 0xFF,
            (session.streamID >> 8) & 0xFF,
            session.streamID & 0xFF
        };
        data.insert(data.end(), 1, 0);
        data.insert(data.end(), 1, (char) eventType);
        data.insert(data.end(), eventData, eventData + 4);

        return data;
    } 

}