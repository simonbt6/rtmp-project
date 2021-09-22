#pragma once
/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-08
 **/

#define WIN32_LEAN_AND_MEAN


#include "RTMPHandshake.hpp"
#include "RTMPChunk.hpp"
#include "Netconnection.hpp"

#include <vector>
#ifdef _WIN32
#include <WinSock2.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#endif

using namespace std;

namespace RTMP
{
    struct Session
    {
        /** 
         * Handshake
         **/
        Handshake::Handshake handshake;

        /** 
         * Last chunk
         **/
        Chunk* lastChunk = nullptr;
        vector<unsigned char> remainingBytes;

        Netconnection::Command* pendingCommand;

        /**
         * Handling
         **/
        SOCKET socket = INVALID_SOCKET;

        int streamID = 0;

        int timestamps = 0;

        int Bandwidth = 4096;

        int totalBytes = 0;


    };
}