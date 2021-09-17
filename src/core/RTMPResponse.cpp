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

        return data;
    }
}