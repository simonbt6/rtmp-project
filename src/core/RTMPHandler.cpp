/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-09-02
 **/

#include "RTMPHandler.hpp"

namespace RTMP
{
    /**
     * Handle received data.
     **/

    void Handler::HandleCommandMessage(Netconnection::Command* command, Session& session)
    {
        switch (command->type)
        {
            case CommandType::Connect:
            {
                Netconnection::Connect* cmd = 
                    dynamic_cast<Netconnection::Connect*>(command);
                

                /**
                 * Send response.
                 **/
                bool success = true;
                
                vector<char> data;
                
                string commandName = success ? "_result" : "_error";
                int transactionID = 1;
                

                unsigned char* commandNameData =
                    Utils::AMF0Encoder::EncodeString(commandName);
                unsigned char* transactionIDData = 
                    Utils::AMF0Encoder::EncodeNumber(transactionID);

                //data.insert(data.end(), commandNameData, commandName.length());
                //data.insert(data.end(), transactionIDData, 8);

                

                SendData(session.socket, data.data(), data.size());

                break;
            };
            
            case CommandType::ConnectResponse:
            {
                Netconnection::ConnectResponse* cmd = 
                    dynamic_cast<Netconnection::ConnectResponse*>(command);
            };
            
            case CommandType::Call:
            {
                Netconnection::Call* cmd = 
                    dynamic_cast<Netconnection::Call*>(command);
            };
            
            case CommandType::CallResponse:
            {
                Netconnection::CallResponse* cmd = 
                    dynamic_cast<Netconnection::CallResponse*>(command);
            };

            case CommandType::CreateStream:
            {
                Netconnection::CreateStream* cmd = 
                    dynamic_cast<Netconnection::CreateStream*>(command);
            };

            case CommandType::CreateStreamResponse:
            {
                Netconnection::CreateStreamResponse* cmd = 
                    dynamic_cast<Netconnection::CreateStreamResponse*>(command);
            };

            case CommandType::OnStatus:
            {
                Netconnection::OnStatus* cmd = 
                    dynamic_cast<Netconnection::OnStatus*>(command);
            };

            case CommandType::Play:
            {
                Netconnection::Play* cmd = 
                    dynamic_cast<Netconnection::Play*>(command);
            };

            case CommandType::Play2:
            {
                Netconnection::Play2* cmd = 
                    dynamic_cast<Netconnection::Play2*>(command);
            };

            case CommandType::DeleteStream:
            {
                Netconnection::DeleteStream* cmd = 
                    dynamic_cast<Netconnection::DeleteStream*>(command);
            };

            case CommandType::ReceiveAudio:
            {
                Netconnection::ReceiveAudio* cmd = 
                    dynamic_cast<Netconnection::ReceiveAudio*>(command);
            };

            case CommandType::ReceiveVideo:
            {
                Netconnection::ReceiveVideo* cmd = 
                    dynamic_cast<Netconnection::ReceiveVideo*>(command);
            };

            case CommandType::Publish:
            {
                Netconnection::Publish* cmd = 
                    dynamic_cast<Netconnection::Publish*>(command);
            };
            
            case CommandType::Seek:
            {
                Netconnection::Seek* cmd = 
                    dynamic_cast<Netconnection::Seek*>(command);
            };

            case CommandType::Pause:
            {
                Netconnection::Pause* cmd = 
                    dynamic_cast<Netconnection::Pause*>(command);
            };

            default:
            {
                printf("\nUnknown command type.");
                break;
            };
        }
    }

    void Handler::HandleVideoMessage(unsigned char*, Session& session)
    {

    }

    void Handler::HandleAudioMessage(unsigned char*, Session& session)
    {

    }


    /**
     * Handle sent data.
     **/

    int Handler::SendData(SOCKET socket, char* data, int length)
    {
        // Only supports win32 for now.
        #ifdef _WIN32
        printf("\nSending %i bytes.", length);
        Utils::FormatedPrint::PrintBytes<char>(data, length);
        return send(socket, data, length, 0);
        #else
        printf("\n\nTCP NOT IMPLEMENTED ON THIS PLATFORM.");
        #endif
        return 0;
    }

    int Handler::SendHandshake(Session& session)
    {
        char* data;
        Handshake::Handshake& handshake = session.handshake;

        switch (handshake.state)
        {
            case Handshake::State::Uninitialized:
            {
                data = new char[1537];

                /**
                 * S0:
                 *  - Version
                 **/
                data[0] = handshake.C0.version;

                /**
                 * S1: 
                 *  - time: 4 bytes.
                 *  - zero: 4 bytes.
                 *  - random bytes: 1528 bytes.
                 **/
                char* randomData = 
                    Utils::BitOperations::generateRandomBytes(RANDOM_BYTES_COUNT);

                for (int i = 0; i < TIME_BYTES_COUNT; i++)
                    data[i + 1] = handshake.C1.time[i];
                for (int i = 0; i < TIME_BYTES_COUNT; i++)
                    data[i + 5] = 0;
                for (int i = 0; i < RANDOM_BYTES_COUNT; i++)
                    data[i + 9] = randomData[i];
                return SendData(session.socket, data, 1537);
                break;
            };

            case Handshake::State::VersionSent:
            {
                return 0;
                break;
            };

            case Handshake::State::AcknowledgeSent:
            {
                data = new char[1536];

                /**
                 * S2:
                 *  - time: 4 bytes.
                 *  - time2: 4 bytes.
                 *  - random bytes: 1528 bytes.
                 **/
                for (int i  = 0; i < TIME_BYTES_COUNT; i++)
                    data[i] = handshake.C1.time[i];
                for (int i = 0; i < TIME_BYTES_COUNT; i++)
                    data[i + 4] = handshake.C1.time[i];
                for (int i = 0; i < RANDOM_BYTES_COUNT; i++)
                    data[i + 8] = handshake.C1.randomBytes[i];
                return SendData(session.socket, data, 1536);
                break;
            };

            case Handshake::State::Done:
            {
                printf("\nHandshake is done. No data to send.");
                return 0;
                break;
            };
        }
        return 0;
    }

    int Handler::SendCommandMessage(Netconnection::Command* command, Session& session)
    {
        int status = 0;
        
        return status;

    }

    void Handler::SendVideoMessage(unsigned char*, Session& session)
    {

    }

    void Handler::SendAudioMessage(unsigned char*, Session& session)
    {

    }
}