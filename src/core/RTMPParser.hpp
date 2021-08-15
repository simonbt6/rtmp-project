/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-14
 * 
 * RTMP packet parser.
 **/
#pragma once

#include "RTMPHandshake.hpp"
#include "rtp.hpp"

#include <iostream>
#include <vector>
using namespace std;

namespace RTMP
{
    class Parser
    {
        private:
            static void ParseF0(vector<int>& data, Handshake::Handshake& handshake);
            static void ParseF1(vector<int>& data, Handshake::Handshake& handshake);
            static void ParseF2(vector<int>& data, Handshake::Handshake& handshake);
            Parser() {};

        public:
            static void ParseHandshake(vector<int>& data, Handshake::Handshake& handshake);
    };
}