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
            static void ParseHandshakeF0(vector<char>& data, Handshake::Handshake& handshake);
            static void ParseHandshakeF1(vector<char>& data, Handshake::Handshake& handshake);
            static void ParseHandshakeF2(vector<char>& data, Handshake::Handshake& handshake);
            
            /**
             * Chunk parsing.
             **/
            static void ParseChunkBasicHeader(vector<char>& data, Chunk& chunk);
            static void ParseChunkMessageHeader(vector<char>& data, Chunk& chunk);
            static void ParseChunkExtendedTimestamp(vector<char>& data, Chunk& chunk);
            static void ParseChunkData(vector<char>& data, Chunk& chunk);

            /**
             * Command parsing.
             **/

            /**
             * 
             **/

            Parser() {};

        public:
            static int ParseData(vector<char>& data, Session& session);
            static int ParseChunk(vector<char>& data, Session& session);
    };
}