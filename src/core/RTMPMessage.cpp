#include "RTMPMessage.hpp"

namespace RTMP
{
        vector<char> ProtocolControlMessage::vSetChunkSize(int chunkSize)
        {
            vector<char> data;
            char bytes[4];

            bytes[0] = (chunkSize >> 24) & 0x7F;
            bytes[1] = ((chunkSize >> 16) & 0xFF);
            bytes[2] = ((chunkSize >> 8) & 0xFF);
            bytes[3] = (chunkSize & 0xFF);

            data.insert(data.end(), bytes, bytes + 4);
            return data;
        }

        vector<char> ProtocolControlMessage::vAbort(int csid)
        {
            vector<char> data;
            char bytes[4];

            bytes[0] = (csid >> 24);
            bytes[1] = ((csid >> 16) & 0xFF);
            bytes[2] = ((csid >> 8) & 0xFF);
            bytes[3] = (csid & 0xFF);

            data.insert(data.end(), bytes, bytes + 4);
            return data;
        }

        vector<char> ProtocolControlMessage::vAcknowledgement(int receivedBytes)
        {
            vector<char> data;
            char bytes[4];

            bytes[0] = (receivedBytes >> 24);
            bytes[1] = ((receivedBytes >> 16) & 0xFF);
            bytes[2] = ((receivedBytes >> 8) & 0xFF);
            bytes[3] = (receivedBytes & 0xFF);

            data.insert(data.end(), bytes, bytes + 4);
            return data;
        }

        vector<char> ProtocolControlMessage::vSetWindowAcknowledgementSize(int size)
        {
            vector<char> data;
            char bytes[4];

            bytes[0] = (size >> 24);
            bytes[1] = ((size >> 16) & 0xFF);
            bytes[2] = ((size >> 8) & 0xFF);
            bytes[3] = (size & 0xFF);

            data.insert(data.end(), bytes, bytes + 4);
            return data;
        }

        vector<char> ProtocolControlMessage::vSetPeerBandwidth(int size, PeerBandwithLimitType type)
        {
            vector<char> data;
            char bytes[5];

            bytes[0] = (size >> 24);
            bytes[1] = ((size >> 16) & 0xFF);
            bytes[2] = ((size >> 8) & 0xFF);
            bytes[3] = (size & 0xFF);
            bytes[4] = (int)(type);

            data.insert(data.end(), bytes, bytes + 5);
            return data;
        }
}