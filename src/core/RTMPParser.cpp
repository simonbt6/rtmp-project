/**
 * Author: Simon Brisebois-Therrien (simonbt6)
 * Date: 2021-08-14
 * 
 * Implementation RTMP packet parser.
 **/

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
        /**
         * - format
         * - chunk stream id
         * - chunk stream id - 64
         **/
        // Byte 0
        unsigned int bZero = (unsigned) data.at(0);

        // format
        unsigned int fmt = (unsigned) bZero >> 6;
        printf("\nFormat value is: %i", fmt);

        // chunk stream id
        unsigned int csid = (unsigned) bZero & 0x3F;
        chunk.basicHeader.baseID = csid;
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
        unsigned char    bTimestamp[3], 
                bMessageLength[3], 
                bMessageTypeId[4]; 
        switch (fmt)
        {
            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type0:
                for (int i = 0; i < 3; i++)
                {
                    bTimestamp[i] = data.at(i + baseI);
                    bMessageLength[i] = data.at(i + baseI + 3);
                }
                // Timestamp delta
                Utils::BitOperations::bytesToInteger<int>(
                    chunk.messageHeader.timestamp_delta,
                    bTimestamp,
                    false);
                // Message length
                Utils::BitOperations::bytesToInteger<int>(
                    chunk.messageHeader.message_length, 
                    bMessageLength, 
                    false);
                // Message type id
                chunk.messageHeader.message_type_id = data.at(6 + baseI);
                for (int i = 0; i < 4; i++)
                    bMessageTypeId[i] = data.at(i + 7 + baseI);
                // Message stream id
                Utils::BitOperations::bytesToInteger<int>(
                    chunk.messageHeader.message_stream_id, 
                    bMessageTypeId,
                    false);
                delete &bTimestamp, &bMessageLength, bMessageTypeId;
                break;

            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type1:
                for (int i = 0; i < 3; i++)
                {
                    bTimestamp[i] = data.at(i + baseI);
                    bMessageLength[i] = data.at(i + baseI + 3);
                }
                // Timestamp delta
                Utils::BitOperations::bytesToInteger<int>(
                    chunk.messageHeader.timestamp_delta,
                    bTimestamp,
                    false);
                // Message length
                Utils::BitOperations::bytesToInteger<int>(
                    chunk.messageHeader.message_length, 
                    bMessageLength, 
                    false);
                // Message type id
                chunk.messageHeader.message_type_id = data.at(6 + baseI);
                delete &bTimestamp, &bMessageLength;
                break;

            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type2:
                for (int i = 0; i < 3; i++)
                    bTimestamp[i] = data.at(i + baseI);
                Utils::BitOperations::bytesToInteger(
                    chunk.messageHeader.timestamp_delta,
                    bTimestamp,
                    false);
                break;

            case ChunkHeader::MessageHeader::ChunkHeaderFormat::Type3:
                // No message header.
                break;

            default:
                printf("\nError: No message header type matching.");
                // Error
                break;
        };
        printf("\nMessage type ID: %i", chunk.messageHeader.message_type_id);
        printf("\nMessage stream ID: %i", chunk.messageHeader.message_stream_id);
        printf("\nMessage Timestamp delta: %i", chunk.messageHeader.timestamp_delta);
    }

    void Parser::ParseChunkExtendedTimestamp(vector<int>& data, Chunk& chunk)
    {

    }

    void Parser::ParseChunkData(vector<int>& data, Chunk& chunk) 
    {

    }

    void Parser::ParseChunk(vector<int>& data, Chunk& chunk)
    {
        ParseChunkBasicHeader(data, chunk);
        ParseChunkMessageHeader(data, chunk);
    }

}