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

    char* ConvertChunkToBytes(Chunk& chunk)
    {
        vector<char> data;

        /**
         * Basic Header. 
         */
        char* basicHeader;
        int basicHeaderLength = 1;

        // 2-63
        if ((2 < chunk.basicHeader.csid) && (chunk.basicHeader.csid < 63))
        {
            basicHeaderLength = 1;
            basicHeader = new char[basicHeaderLength];

            basicHeader[0] = (chunk.basicHeader.fmt << 6) + (chunk.basicHeader.csid);
        }
        // 64-319
        else if ((319 >= chunk.basicHeader.csid) && (chunk.basicHeader.csid > 63))
        {
            basicHeaderLength = 2;
            basicHeader = new char[basicHeaderLength];

            basicHeader[0] = chunk.basicHeader.fmt << 6;
            basicHeader[1] = 0;
            basicHeader[2] = (chunk.basicHeader.csid - 64);
        }
        else if ((65559 > chunk.basicHeader.csid) && (chunk.basicHeader.csid > 319))
        {
            basicHeaderLength = 3;
            basicHeader = new char[basicHeaderLength];

            basicHeader[0] = (chunk.basicHeader.fmt << 6) + 1;
            basicHeader[1] = 0xFF;
            basicHeader[2] = (chunk.basicHeader.csid - 0xFF) / 256;
        }
        else
            printf("\nError, chunk stream id too big. %i", chunk.basicHeader.csid);
        data.insert(data.end(), basicHeader, basicHeader + basicHeaderLength);

        /**
         * Message Header. 
         */
        switch (chunk.basicHeader.fmt)
        {
            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type0:
            {
                char* timestamp = new char[3];
                char* message_length = new char[3];
                char* message_type_id = new char[1];
                char* message_stream_id = new char[3];

                timestamp[0] = chunk.messageHeader.timestamp_delta >> 16;
                timestamp[1] = (chunk.messageHeader.timestamp_delta >> 8) & 0xFF;
                timestamp[3] = chunk.messageHeader.timestamp_delta & 0xFF;
 
                message_length[0] = chunk.messageHeader.message_length >> 16;
                message_length[1] = (chunk.messageHeader.message_length >> 8) & 0xFF;
                message_length[3] = chunk.messageHeader.message_length & 0xFF;

                message_type_id[0] = chunk.messageHeader.message_type_id;

                message_stream_id[0] = chunk.messageHeader.message_stream_id >> 16;
                message_stream_id[1] = (chunk.messageHeader.message_stream_id >> 8) & 0xFF;
                message_stream_id[3] = chunk.messageHeader.message_stream_id & 0xFF;

                data.insert(data.end(), timestamp, timestamp + 3);
                data.insert(data.end(), message_length, message_length + 3);
                data.insert(data.end(), message_type_id, message_type_id + 1);
                data.insert(data.end(), message_stream_id, message_stream_id + 3);
                break;
            };
            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type1:
            {
                char* timestamp_delta = new char[3];
                char* message_length = new char[3];
                char* message_type_id = new char[1];

                timestamp_delta[0] = chunk.messageHeader.timestamp_delta >> 16;
                timestamp_delta[1] = (chunk.messageHeader.timestamp_delta >> 8) & 0xFF;
                timestamp_delta[3] = chunk.messageHeader.timestamp_delta & 0xFF;

                message_length[0] = chunk.messageHeader.message_length >> 16;
                message_length[1] = (chunk.messageHeader.message_length >> 8) & 0xFF;
                message_length[3] = chunk.messageHeader.message_length & 0xFF;

                message_type_id[0] = chunk.messageHeader.message_type_id;

                data.insert(data.end(), timestamp_delta, timestamp_delta + 3);
                data.insert(data.end(), message_length, message_length + 3);
                data.insert(data.end(), message_type_id, message_type_id + 1);
                break;
            };
            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type2:
            {
                char* timestamp_delta = new char[3];

                timestamp_delta[0] = chunk.messageHeader.timestamp_delta >> 16;
                timestamp_delta[1] = (chunk.messageHeader.timestamp_delta >> 8) & 0xFF;
                timestamp_delta[3] = chunk.messageHeader.timestamp_delta & 0xFF;
                
                data.insert(data.end(), timestamp_delta, timestamp_delta + 3);
                break;
            };
            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type3:
            {
                // No message header.
                break;
            };
        }

        chunk.displacement = data.size();
        return data.data();
    }

    int Handler::SendChunk(char* data, int length, Session& session)
    {
        Chunk* _chunk = session.lastChunk;
        
        // Chunk to send.
        Chunk chunk;

        // Basic header.
        chunk.basicHeader.fmt = 0;
        chunk.basicHeader.csid = _chunk->basicHeader.csid;

        // Message header.
        chunk.messageHeader.message_length = length;
        chunk.messageHeader.message_stream_id = _chunk->messageHeader.message_stream_id;
        chunk.messageHeader.message_type_id = _chunk->messageHeader.message_type_id;
        chunk.messageHeader.timestamp_delta = _chunk->messageHeader.timestamp_delta;

        // Chunk payload.
        chunk.data = reinterpret_cast<unsigned char*>(data);

        char* chunkData = ConvertChunkToBytes(chunk);

        return SendData(session.socket, chunkData, chunk.displacement);
    }

    /**
     * Handle received data.
     **/

    int Handler::HandleCommandMessage(Netconnection::Command* command, Session& session)
    {
        if (command == nullptr) return -1;

        if (Netconnection::Connect* cmd = dynamic_cast<Netconnection::Connect*>(command))
        {
                printf("\n\nConnect command response.");               

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

                for (int i = 0; i < commandNameData.size; i++)
                    data.push_back(commandNameData.data[i]);
                for (int i = 0; i < transactionIDData.size; i++)
                    data.push_back(transactionIDData.data[i]);
                return SendChunk(data.data(), data.size(), session);
        }
        else if (Netconnection::ConnectResponse* cmd = dynamic_cast<Netconnection::ConnectResponse*>(command))
        {

        }
        else if (Netconnection::Call* cmd = dynamic_cast<Netconnection::Call*>(command))
        {
            
        }
        else if (Netconnection::CallResponse* cmd = dynamic_cast<Netconnection::CallResponse*>(command))
        {
            
        }
        else if (Netconnection::CreateStream* cmd = dynamic_cast<Netconnection::CreateStream*>(command))
        {
            
        }
        else if (Netconnection::CreateStreamResponse* cmd = dynamic_cast<Netconnection::CreateStreamResponse*>(command))
        {
            
        }
        else if (Netconnection::OnStatus* cmd = dynamic_cast<Netconnection::OnStatus*>(command))
        {
            
        }
        else if (Netconnection::Play* cmd = dynamic_cast<Netconnection::Play*>(command))
        {
            
        }
        else if (Netconnection::Play2* cmd = dynamic_cast<Netconnection::Play2*>(command))
        {
            
        }
        else if (Netconnection::DeleteStream* cmd = dynamic_cast<Netconnection::DeleteStream*>(command))
        {
            
        }
        else if (Netconnection::ReceiveAudio* cmd = dynamic_cast<Netconnection::ReceiveAudio*>(command))
        {
            
        }
        else if (Netconnection::ReceiveVideo* cmd = dynamic_cast<Netconnection::ReceiveVideo*>(command))
        {
            
        }
        else if (Netconnection::Publish* cmd = dynamic_cast<Netconnection::Publish*>(command))
        {
            
        }
        else if (Netconnection::Seek* cmd = dynamic_cast<Netconnection::Seek*>(command))
        {
            
        }
        else if (Netconnection::Pause* cmd = dynamic_cast<Netconnection::Pause*>(command))
        {
            
        }
        else 
        {
            printf("\nUnknown command type.");
        }
        return 0;
    }

    void Handler::HandleVideoMessage(unsigned char*, Session& session)
    {

    }

    void Handler::HandleAudioMessage(unsigned char*, Session& session)
    {

    }

    int Handler::HandleChunk(Chunk& chunk, Session& session)
    {
        int status = 0;
        /** 
         * Determine message type.
         */
        if (chunk.messageHeader.message_type_id == 0)
        {
            // Idk if this is possible.
            printf("\nMessage type ID of 0.");
        }
        else if (6 >= chunk.messageHeader.message_type_id)
        {
            // Protocol control message.
            switch (chunk.messageHeader.message_type_id)
            {
                case ProtocolControlMessage::Type::SetChunkSize:
                {
                    int chunksize = 0;
                    Utils::BitOperations::bytesToInteger(
                        chunksize, 
                        chunk.data, 
                        false, 
                        chunk.messageHeader.message_length);
                    printf("\nProtocol control message: Set chunk size %i.", chunksize);
                    break;
                };
                case ProtocolControlMessage::Type::Abort:
                {
                    int csid = 0;
                    Utils::BitOperations::bytesToInteger(
                        csid,
                        chunk.data,
                        false,
                        chunk.messageHeader.message_length
                    );
                    printf("\nProtocol control message: Abort. Stream ID: %i", csid);
                    break;
                };
                case ProtocolControlMessage::Type::Acknowledgement:
                {
                    printf("\nProtocol control message: Acknowledgement.");
                    break;
                };
                case ProtocolControlMessage::Type::WindowAcknowledgementSize:
                {
                    printf("\nProtocol control message: Window Acknowledgement size.");
                    break;
                }
                case ProtocolControlMessage::Type::SetPeerBandwidth:
                {
                    printf("\nProtocol control message: Set peer Bandwidth.");
                    break;
                }
            };
        }
        // Normal message.
        else
        {
            switch (chunk.messageHeader.message_type_id)
            {
                case Message::Type::AudioMessage:
                    printf("\n\nAudio message.");
                    break;
                case Message::Type::VideoMessage:
                    printf("\n\nVideo message.");
                    break;
                case Message::Type::AggregateMessage:
                    printf("\n\nAggregate message.");
                    break;
                case Message::Type::AMF0CommandMessage:
                {
                    printf("\n\nAMF0 Command message.");
                    Netconnection::Command* command = Utils::AMF0Decoder::DecodeCommand(
                        chunk.data, 
                        chunk.messageHeader.message_length);
                    status = Handler::HandleCommandMessage(command, session);
                    printf("\n");
                    break;
                }
                case Message::Type::AMF3CommandMessage:
                    printf("\n\nAMF3 Command message.");
                    break;
                case Message::Type::AMF0DataMessage:
                    printf("\n\nAMF0 Data message.");
                    break;
                case Message::Type::AMF3DataMessage:
                    printf("\n\nAMF3 Data message.");
                    break;
                case Message::Type::AMF0SharedObjectMessage:
                    printf("\n\nAMF0 Shared object message.");
                    break;
                case Message::Type::AMF3SharedObjectMessage:
                    printf("\n\nAMF3 Shared object message.");
                    break;
            }
        }

        return status;
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