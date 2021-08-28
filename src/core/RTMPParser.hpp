/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-14
 * 
 * RTMP packet parser.
 **/
#pragma once

#include "RTMPHandshake.hpp"
#include "RTMPMessage.hpp"
#include "RTMPChunk.hpp"
#include "../utils/Bit.hpp"
#include "../utils/FormatedPrint.hpp"
#include "../utils/amf0.hpp"
#include "rtp.hpp"

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
            static void ParseF0(vector<int>& data, Handshake::Handshake& handshake);
            static void ParseF1(vector<int>& data, Handshake::Handshake& handshake);
            static void ParseF2(vector<int>& data, Handshake::Handshake& handshake);
            
            /**
             * Chunk parsing.
             **/
            static void ParseChunkBasicHeader(vector<int>& data, Chunk& chunk);
            static void ParseChunkMessageHeader(vector<int>& data, Chunk& chunk);
            static void ParseChunkExtendedTimestamp(vector<int>& data, Chunk& chunk);
            static void ParseChunkData(vector<int>& data, Chunk& chunk);

            /**
             * Command parsing.
             **/

            /**
             * 
             **/

            Parser() {};

        public:
            static void ParseHandshake(vector<int>& data, Handshake::Handshake& handshake);
            static void ParseChunk(vector<int>& data, Chunk& chunk);
    };
}