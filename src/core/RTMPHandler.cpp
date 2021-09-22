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
        Utils::FormatedPrint::PrintFormated(
            "Handler::SendData", 
            "Sending " + to_string(length) + " bytes.");

        return send(socket, data, length, 0);
        #else
        Utils::FormatedPrint::PrintError(
            "Handler::SendData", 
            "TCP NOT IMPLEMENTED ON THIS CPU ARCHITECTURE.");
        #endif
        return 0;
    }

    vector<char> ConvertChunkToBytes(Chunk& chunk, char* body, int length)
    {
        vector<char> data;

        /**
         * Basic Header. 
         */
        char* basicHeader;
        int basicHeaderLength = 1;

        // 2-63
        if (chunk.basicHeader.csid < 63)
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
            Utils::FormatedPrint::PrintError(
                "Handler::ConvertChunkToBytes", 
                "Error, chunk stream id too big. " + to_string(length) + ".");

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
                char* message_stream_id = new char[4];

                timestamp[0] = (chunk.messageHeader.timestamp_delta >> 16);
                timestamp[1] = ((chunk.messageHeader.timestamp_delta >> 8) & 0xFF);
                timestamp[2] = (chunk.messageHeader.timestamp_delta & 0xFF);
 
                message_length[0] = (chunk.messageHeader.message_length >> 16);
                message_length[1] = ((chunk.messageHeader.message_length >> 8) & 0xFF);
                message_length[2] = (chunk.messageHeader.message_length & 0xFF);

                message_type_id[0] = chunk.messageHeader.message_type_id;

                message_stream_id[0] = (chunk.messageHeader.message_stream_id >> 24);
                message_stream_id[1] = (chunk.messageHeader.message_stream_id >> 16);
                message_stream_id[2] = (chunk.messageHeader.message_stream_id >> 8) & 0xFF;
                message_stream_id[3] = (chunk.messageHeader.message_stream_id & 0xFF);

                data.insert(data.end(), timestamp, timestamp + 3);
                data.insert(data.end(), message_length, message_length + 3);
                data.insert(data.end(), message_type_id, message_type_id + 1);
                data.insert(data.end(), message_stream_id, message_stream_id + 4);
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

        data.insert(data.end(), body, body + length);

        chunk.displacement = data.size();
        return data;
    }

    int Handler::SendChunk(char* data, int length, Session& session, int message_type)
    {
        Utils::FormatedPrint::PrintFormated(
            "Handler::SendChunk", 
            "Sending " + to_string(length) + " bytes.");

        Chunk* _chunk = session.lastChunk;
        
        // Chunk to send.
        Chunk chunk;

        if (message_type > 0 && message_type <= 6)
        {
            // Message control protocol
            chunk.basicHeader.fmt = 0;
            chunk.basicHeader.csid = 2;
            chunk.messageHeader.message_type_id = message_type;
            chunk.messageHeader.message_stream_id = 0;
        }
        else
        {
            // Other message.
            chunk.basicHeader.fmt = _chunk->basicHeader.fmt;
            chunk.basicHeader.csid = _chunk->basicHeader.csid;
            chunk.messageHeader.message_type_id = _chunk->messageHeader.message_type_id;
            chunk.messageHeader.message_stream_id = _chunk->messageHeader.message_stream_id;
        }


        // Message header.
        chunk.messageHeader.message_length = length;
        chunk.messageHeader.timestamp_delta = _chunk->messageHeader.timestamp_delta;

        // Chunk payload.
        chunk.data = reinterpret_cast<unsigned char*>(data);

        vector<char> chunkData = ConvertChunkToBytes(chunk, data, length);
        return SendData(session.socket, chunkData.data(), chunk.displacement);
    }

    /**
     * Handle received data.
     **/

    int Handler::HandleCommandMessage(Netconnection::Command* command, Session& session)
    {
        int status = 0;
        if (command == nullptr) return -1;

        if (Netconnection::Connect* cmd = dynamic_cast<Netconnection::Connect*>(command))
        {

            Utils::FormatedPrint::PrintFormated(
                "Handler::HandleCommandMessage", 
                "Connect command response.");          

            session.pendingCommand = cmd;

            status += InitializeConnect(session);

            Utils::FormatedPrint::PrintInfo("Initialize done.");

                        
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
            vector<char> data;
            Utils::FormatedPrint::PrintFormated(
                "Handler::HandleCommandMessage", 
                "Handling response for create stream command message."
            );

            /**
             * @brief 
             * createStream response -> _result
             * UserControl -> StreamBegin
             * 
             */
            data = RTMP::ServerResponse::StreamBegin(session);
            status += SendChunk(data.data(), data.size(), session, 0x04);

            data = RTMP::ServerResponse::PublishResponse(session);
            status += SendChunk(data.data(), data.size(), session, 0x14);
            
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
        else if (Netconnection::ReleaseStream* cmd = dynamic_cast<Netconnection::ReleaseStream*>(command))
        {
            Utils::FormatedPrint::PrintFormated(
                "Handler::HandleCommandMessage",
                "Release Stream command message."
            );
        }
        else if (Netconnection::FCPublish* cmd = dynamic_cast<Netconnection::FCPublish*>(command))
        {
            Utils::FormatedPrint::PrintFormated(
                "Handler::HandleCommandMessage",
                "FCPublish command message."
            );
            /**
             * @brief 
             * publish response -> _result
             * 
             */
        }
        else 
        {
            Utils::FormatedPrint::PrintError(
                "Handler::HandleCommandMessage", 
                "Unknown command type.");
        }
        return status;
    }

    void Handler::HandleVideoMessage(unsigned char* data, Session& session)
    {

    }

    void Handler::HandleAudioMessage(unsigned char* data, Session& session)
    {

    }

    int Handler::InitializeConnect(Session& session)
    {
        int status = 0;

        /**
         * Initialization: 
         *  - Window acknowledge size
         *  - Set peer bandwidth
         *  - Set chunk size
         *  - connect response (_result)
         * 
         **/

        /**
         * Window acknowledge size
         */
        vector<char> windowAckData = ProtocolControlMessage::vSetWindowAcknowledgementSize(4096);
        status += SendChunk(windowAckData.data(), windowAckData.size(), session, (int)ProtocolControlMessage::Type::WindowAcknowledgementSize);

        /**
         * Set Peer Bandwith
         */
        vector<char> setPeerBandwidthData = ProtocolControlMessage::vSetPeerBandwidth(4096, ProtocolControlMessage::PeerBandwithLimitType::Hard);
        status += SendChunk(setPeerBandwidthData.data(), setPeerBandwidthData.size(), session, (int)ProtocolControlMessage::Type::SetPeerBandwidth);

        /**
         * Set Chunk Size
         */
        vector<char> setChunkSizeData = ProtocolControlMessage::vSetChunkSize(4096);
        status += SendChunk(setChunkSizeData.data(), setChunkSizeData.size(), session, (int)ProtocolControlMessage::Type::SetChunkSize);

        /**
         * Connect Response (_result)
         */
        vector<char> connectResponseData = ServerResponse::ConnectResponse(session);
        status += SendChunk(connectResponseData.data(), connectResponseData.size(), session, 0x14);

        return status;
        
    }

    int Handler::HandleChunk(Chunk& chunk, Session& session)
    {
        Utils::FormatedPrint::PrintBytes<unsigned char>(chunk.data, chunk.messageHeader.message_length);
        int status = 0;
        /** 
         * Determine message type.
         */
        if (chunk.messageHeader.message_type_id == 0)
        {
            // Idk if this is possible.
            Utils::FormatedPrint::PrintFormated(
                "Handler::HandleChunk", 
                "Message type ID of 0.");
        }
        else if (chunk.messageHeader.message_type_id == 4)
        {
            Utils::FormatedPrint::PrintFormated(
                "Handler::HandleChunk", 
                "User control message.");
        }
        else if (6 >= chunk.messageHeader.message_type_id)
        {
            // Protocol control message.
            switch (chunk.messageHeader.message_type_id)
            {
                case ProtocolControlMessage::Type::SetChunkSize:
                {
                    int chunksize = 0;
                    for (int i = 0; i < 16; i++)
                        printf("\n%i", chunk.data[i]);
                    Utils::BitOperations::bytesToInteger(
                        chunksize, 
                        session.lastChunk->data, 
                        false, 
                        chunk.messageHeader.message_length);
                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "Protocol control message: Set chunk size -> " + to_string(chunksize) + ".");

                    session.Bandwidth = chunksize;
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
                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "Protocol control message: Abort. Stream ID: " + to_string(csid) + ".");
                    // vector<char> data = ProtocolControlMessage::vAbort(csid);
                    // SendChunk(data.data(), data.size(), session, ProtocolControlMessage::Abort);
                    break;
                };
                case ProtocolControlMessage::Type::Acknowledgement:
                {
                    // int receivedData = session.totalBytes;
                    int seqnumber = 0;
                    Utils::BitOperations::bytesToInteger(
                        seqnumber, 
                        session.lastChunk->data, 
                        false, 
                        chunk.messageHeader.message_length);
                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "Protocol control message: Acknowledgement. Sequence number -> " + to_string(seqnumber));
                    break;
                };
                case ProtocolControlMessage::Type::WindowAcknowledgementSize:
                {
                    int WindowAcknowledgementSize = session.Bandwidth;

                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "Protocol control message: Window Acknowledgement size.");

                    // status = Handler::HandleCommandMessage(session.pendingCommand, session);
                    break;
                }
                case ProtocolControlMessage::Type::SetPeerBandwidth:
                {
                    int bandwith = session.Bandwidth;

                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "Protocol control message: Set peer Bandwidth.");

                    // vector<char> data = ProtocolControlMessage::vSetPeerBandwidth(bandwith, ProtocolControlMessage::PeerBandwithLimitType::Hard);
                    // SendChunk(data.data(), data.size(), session, ProtocolControlMessage::SetPeerBandwidth);
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
                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "Audio message.");
                    break;
                case Message::Type::VideoMessage:
                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "Video message.");
                    break;
                case Message::Type::AggregateMessage:
                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "Aggregate message.");
                    break;
                case Message::Type::AMF0CommandMessage:
                {
                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "AMF0 Command message.");
                    Utils::FormatedPrint::PrintBytes<unsigned char>(chunk.data, chunk.messageHeader.message_length);
                    
                    Netconnection::Command* command = Utils::AMF0Decoder::DecodeCommand(
                        chunk.data, 
                        chunk.messageHeader.message_length);
                    session.pendingCommand = command;
                    
                    status += HandleCommandMessage(command, session);

                    printf("\n");
                    break;
                }
                case Message::Type::AMF3CommandMessage:
                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "AMF3 Command message.");
                    break;
                case Message::Type::AMF0DataMessage:
                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "AMF0 Data message.");
                    break;
                case Message::Type::AMF3DataMessage:
                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "AMF3 Data message.");
                    break;
                case Message::Type::AMF0SharedObjectMessage:
                    Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "AMF0 Shared object message.");
                    break;
                case Message::Type::AMF3SharedObjectMessage:
                Utils::FormatedPrint::PrintFormated(
                        "Handler::HandleChunk", 
                        "AMF3 Shared object message.");
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
                Utils::FormatedPrint::PrintFormated(
                        "Handler::SendHandshake", 
                        "Handshake is done. No data to send.");
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