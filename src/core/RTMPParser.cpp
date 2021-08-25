/**
 * Author: Simon Brisebois-Therrien (simonbt6)
 * Date: 2021-08-14
 * 
 * Implementation RTMP packet parser.
 **/
#define __DEBUG true

#include "RTMPParser.hpp"

using namespace std;

namespace RTMP {

    /**
     * Version: 1 byte
     **/
    void Parser::ParseF0(vector<int>& data, Handshake::Handshake& handshake)
    {
        handshake.C0.version = (unsigned short int) data.at(0);        
    }

    /**
     * Time: 4 bytes.
     * Zeros: 4 bytes.
     * Random bytes: 1528 bytes.
     **/
    void Parser::ParseF1(vector<int>& data, Handshake::Handshake& handshake)
    {
        // Time
        for (int i = 0; i < TIME_BYTES_COUNT + 1; i++)
            handshake.C1.time[i] = data.at(i + 1);

        // Random bytes
        for (int i = 0; i < RANDOM_BYTES_COUNT; i++)
            handshake.C1.randomBytes[i] = data.at(i + 9);
    }

    /**
     * Time: 4 bytes.
     * Time2: 4 bytes.
     * Random bytes: 1528 bytes.
     **/
    void Parser::ParseF2(vector<int>& data, Handshake::Handshake& handshake)
    {
        for (int i = 0; i < TIME_BYTES_COUNT; i++) 
            handshake.C2.time[i] = data.at(i);
        for (int i = 0; i < TIME_BYTES_COUNT; i++)
            handshake.C2.time2[i] = data.at(i + TIME_BYTES_COUNT);
        for (int i = 0; i < RANDOM_BYTES_COUNT; i++)
            handshake.C2.randomBytes[i] = data.at(i + (2*TIME_BYTES_COUNT));
    }

    void Parser::ParseHandshake(vector<int>& data, Handshake::Handshake& handshake)
    {
        int size = data.size();
        
        // F0 & F1
        if (size == 1537)
        {
            Parser::ParseF0(data, handshake);
            Parser::ParseF1(data, handshake);
            handshake.state = Handshake::State::VersionSent;
        }
        // F1
        else if (size == 1536)
        {
            Parser::ParseF1(data, handshake);
            handshake.state = Handshake::State::AcknowledgeSent;
        }
        // Probably F2
        else
        {
            Parser::ParseF2(data, handshake);
            handshake.state = Handshake::State::DoneSent;
        }
    }


    /**
     * Chunk Parsing.
     **/

    void Parser::ParseChunkBasicHeader(vector<int>& data, Chunk& chunk)
    {
        // Byte 0
        unsigned int bZero = (unsigned) data.at(0);

        // format
        unsigned int fmt = (unsigned) bZero >> 6;
        printf("\nFormat value is: %i", fmt);

        // chunk stream id
        unsigned int csid = (unsigned) bZero & 0x3F;
        chunk.basicHeader.baseID = 2;
        // If the chunk stream id is of 0 or 1,
        // it's a 2 or 3 bytes header field.
        if (csid == 0) 
            csid = data.at(1) + 64;
        else if (csid == 1) 
            csid = ((data.at(2))*256 + (data.at(1) + 64));

        printf("\nChunk stream id: %i", csid);

        // Assignations
        chunk.basicHeader.csid = csid;
        chunk.basicHeader.fmt = fmt;
    }

    void Parser::ParseChunkMessageHeader(vector<int>& data, Chunk& chunk)
    {
        int fmt = chunk.basicHeader.fmt;
        int baseI = (chunk.basicHeader.baseID * -1) + 3;
        chunk.displacement += baseI;
        unsigned char   bTimestamp[3], 
                        bMessageLength[3], 
                        bMessageTypeId[4]; 
        switch (fmt)
        {
            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type0:
                // 11-byte message header.
                chunk.displacement += 11;
                for (int i = 0; i < 3; i++) //  3 bytes
                {
                    bTimestamp[i] = data.at(i + baseI);
                    bMessageLength[i] = data.at(i + baseI + 3);
                }
                for (int i = 0; i < 4; i++) //  4 bytes
                    bMessageTypeId[i] = data.at(i + 7 + baseI);

                // Message type id -            1 byte
                chunk.messageHeader.message_type_id = data.at(6 + baseI);
                // Timestamp delta
                Utils::BitOperations::bytesToInteger<int>(
                    chunk.messageHeader.timestamp_delta,
                    bTimestamp,
                    false,
                    3);
                // Message length
                Utils::BitOperations::bytesToInteger<int>(
                    chunk.messageHeader.message_length, 
                    bMessageLength, 
                    false,
                    3);
                // Message stream id
                Utils::BitOperations::bytesToInteger<int>(
                    chunk.messageHeader.message_stream_id, 
                    bMessageTypeId,
                    false,
                    4);
                //delete &bTimestamp, &bMessageLength, &bMessageTypeId;
                break;
                

            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type1:
                // 7-byte message header.
                chunk.displacement += 7;
                for (int i = 0; i < 3; i++)
                {
                    bTimestamp[i] = data.at(i + baseI);
                    bMessageLength[i] = data.at(i + baseI + 3);
                }
                // Timestamp delta
                Utils::BitOperations::bytesToInteger<int>(
                    chunk.messageHeader.timestamp_delta,
                    bTimestamp,
                    false,
                    3);
                // Message length
                Utils::BitOperations::bytesToInteger<int>(
                    chunk.messageHeader.message_length, 
                    bMessageLength, 
                    false,
                    3);
                // Message type id
                chunk.messageHeader.message_type_id = data.at(6 + baseI);
                //delete &bTimestamp, &bMessageLength;
                break;

            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type2:
                // 3-byte message header.
                chunk.displacement += 3;
                for (int i = 0; i < 3; i++)
                    bTimestamp[i] = data.at(i + baseI);
                Utils::BitOperations::bytesToInteger<int>(
                    chunk.messageHeader.timestamp_delta,
                    bTimestamp,
                    false,
                    3);
                break;

            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type3:
                // No message header.
                printf("\nType3 header format: No message header.");
                break;

            default:
                printf("\nError: No message header type matching.");
                // Error
                break;
        };
    }

    void Parser::ParseChunkExtendedTimestamp(vector<int>& data, Chunk& chunk)
    {
        if (!(chunk.messageHeader.timestamp_delta == 0xFFFFFF)) 
            return;
        unsigned char bExtendedTimestamp[4];
        for (int i = 0; i < 4; i++)
            bExtendedTimestamp[i] = data.at(i + chunk.displacement);
        
        Utils::BitOperations::bytesToInteger(
            chunk.extendedTimestamp,
            bExtendedTimestamp,
            false,
            4); 

        printf("\nExtended Timestamp: %i", chunk.extendedTimestamp);
        chunk.displacement += 4;
    }

    void Parser::ParseChunkData(vector<int>& data, Chunk& chunk) 
    {
        int size = data.size() - chunk.displacement;

        printf("\nPayload size byte : %i", size);

        unsigned char* bData = new unsigned char[size];
        for (int i = 0; i < size; i++)
            bData[i] = (unsigned)data.at(i + chunk.displacement);
        
        chunk.data = bData;
        
    }

    void Parser::ParseChunk(vector<int>& data, Chunk& chunk)
    {
        ParseChunkBasicHeader(data, chunk);
        ParseChunkMessageHeader(data, chunk);
        ParseChunkExtendedTimestamp(data, chunk);
        ParseChunkData(data, chunk);

        /** 
         * Determine message type.
         */
        if (chunk.messageHeader.message_type_id == 0)
        {
            // Idk if this is possible.
        }
        else if (6 >= chunk.messageHeader.message_type_id)
        {
            int chunksize = 0;
            // Protocol control message.
            switch (chunk.messageHeader.message_type_id)
            {
                case ProtocolControlMessage::Type::SetChunkSize:
                    Utils::BitOperations::bytesToInteger(
                        chunksize, 
                        chunk.data, 
                        false, 
                        chunk.messageHeader.message_length);
                    printf("\nProtocol control message: Set chunk size %i.", chunksize);
                    break;
                case ProtocolControlMessage::Type::Abort:
                    printf("\nProtocol control message: Abort.");
                    break;
                case ProtocolControlMessage::Type::Acknowledgement:
                    printf("\nProtocol control message: Acknowledgement.");
                    break;
                case ProtocolControlMessage::Type::WindowAcknowledgementSize:
                    printf("\nProtocol control message: Window Acknowledgement size.");
                    break;
                case ProtocolControlMessage::Type::SetPeerBandwidth:
                    printf("\nProtocol control message: Set peer Bandwidth.");
                    break;
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
                    printf("\n\nAMF0 Command message.");
                    Utils::AMF0Decoder::DecodeCommand(
                        chunk.data, 
                        chunk.messageHeader.message_length);
                    break;
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

        #if (__DEBUG == true)
        printf("\nMessage timestamp delta: %i", chunk.messageHeader.timestamp_delta);
        printf("\nMessage type ID: %i", chunk.messageHeader.message_type_id);
        printf("\nMessage length : %i", chunk.messageHeader.message_length);
        printf("\nMessage stream ID: %i", chunk.messageHeader.message_stream_id);
        #endif
    }

}