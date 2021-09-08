/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-09-02
 **/

#include "RTMPHandler.hpp"

namespace RTMP
{
    /**
     * Handle sent data.
     **/

    int Handler::SendData(SOCKET socket, char* data, int length)
    {
        // Only supports win32 for now.
        #ifdef _WIN32
        printf("\nSending %i bytes.", length);
        //Utils::FormatedPrint::PrintBytes<char>(data, length);
        return send(socket, data, length, 0);
        #else
        printf("\n\nTCP NOT IMPLEMENTED ON THIS PLATFORM.");
        #endif
        return 0;
    }

    /**
     * Handle received data.
     **/

    int Handler::HandleCommandMessage(Netconnection::Command* command, Session& session)
    {
        if (command == nullptr) return -1;
        
        switch (command->type)
        {
            /**
             * Sends a connect response.
             **/
            case CommandType::Connect:
            {
                printf("\n\nConnect command response.");
                Netconnection::Connect* cmd = 
                    dynamic_cast<Netconnection::Connect*>(command);
                

                /**
                 * Send response.
                 **/
                bool success = true;
                
                vector<char> data;
                
                string commandName = success ? "_result" : "_error";
                int transactionID = 1;
                

                Utils::AMF0::Data commandNameData =
                    Utils::AMF0Encoder::EncodeString(commandName);

                Utils::AMF0::Data transactionIDData = 
                    Utils::AMF0Encoder::EncodeNumber(transactionID);
                

                data.insert(data.end(), reinterpret_cast<char*>(commandNameData.data), reinterpret_cast<char*>(commandNameData.data)+commandNameData.size);
                data.insert(data.end(), reinterpret_cast<char*>(transactionIDData.data), reinterpret_cast<char*>(transactionIDData.data)+transactionIDData.size);

                return SendData(session.socket, data.data(), data.size());

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
        return 0;
    }

    void Handler::HandleVideoMessage(unsigned char*, Session& session)
    {

    }

    void Handler::HandleAudioMessage(unsigned char*, Session& session)
    {

    }

    int Handler::SendHandshake(Session& session)
    {
        char* data;
        Handshake::Handshake& handshake = session.handshake;

        switch (handshake.state)
        {
            case Handshake::State::Uninitialized:
            {
                data = new char[3073];

                /**
                 * S1: 
                 *  - time: 4 bytes.
                 *  - zero: 4 bytes.
                 *  - random bytes: 1528 bytes.
                 **/
                char* randomData = Utils::BitOperations::GenerateRandom8BitBytes(RANDOM_BYTES_COUNT);

                // S0
                data[0] = handshake.C0.version;
                
                for (int i = 0; i < TIME_BYTES_COUNT; i++)
                {
                    // S1
                    data[i + 1] = 0;

                    // S2
                    data[i + 1537] = handshake.C1.time[i];
                    data[i + 1537 + TIME_BYTES_COUNT] = handshake.C1.time[i];
                }

                for (int i = 0; i < RANDOM_BYTES_COUNT; i++)
                {
                    // S1
                    data[i + 5] = randomData[i];

                    // S2
                    data[i + 1537 + (2 * TIME_BYTES_COUNT)] = handshake.C1.randomBytes[i];
                }

                return SendData(session.socket, data, 3073);
                break;
            };

            case Handshake::State::VersionSent:
            {
                return 0;
                break;
            };

            case Handshake::State::AcknowledgeSent:
            {

                return 0;
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