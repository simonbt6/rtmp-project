/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-14
 * 
 * RTMP packet parser.
 **/
#pragma once

#include "RTMPSession.hpp"
#include "RTMPHandshake.hpp"
#include "RTMPMessage.hpp"
#include "RTMPChunk.hpp"
#include "../utils/Bit.hpp"
#include "../utils/FormatedPrint.hpp"
#include "../utils/amf0.hpp"

#include <iostream>
#include <vector>
using namespace std;

namespace RTMP
{
    class Parser
    {
        private:

            /** 
             * Handshake parsing.
             **/
            static void ParseHandshakeF0(vector<unsigned char>& data, Handshake::Handshake& handshake);
            static void ParseHandshakeF1(vector<unsigned char>& data, Handshake::Handshake& handshake);
            static void ParseHandshakeF2(vector<unsigned char>& data, Handshake::Handshake& handshake);
            
            /**
             * Chunk parsing.
             **/
            static void ParseChunkBasicHeader(vector<unsigned char>& data, Chunk& chunk);
            static void ParseChunkMessageHeader(vector<unsigned char>& data, Chunk& chunk);
            static void ParseChunkExtendedTimestamp(vector<unsigned char>& data, Chunk& chunk);
            static void ParseChunkData(vector<unsigned char>& data, Chunk& chunk);

            /**
             * Command parsing.
             **/

            /**
             * 
             **/

            Parser() {};

        public:
            static int ParseData(vector<unsigned char>& data, Session& session);
            static int ParseChunk(vector<unsigned char>& data, Session& session);
            static int ParseChunks(vector<unsigned char>& data, Session& session);
    };
}