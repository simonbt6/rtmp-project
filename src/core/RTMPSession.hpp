#pragma once
/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-08
 **/

#define _HAS_STD_BYTE 0
#define WIN32_LEAN_AND_MEAN


#include "RTMPHandshake.hpp"

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
         * Handling
         **/
        SOCKET socket = INVALID_SOCKET;

        int timestamps = 0;

        int ChunkSize = 128;

        int Bandwidth = 4096;


    };
}