#pragma once
/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-09-02
 **/

#define _HAS_STD_BYTE 0
#define WIN32_LEAN_AND_MEAN

#include "Netconnection.hpp"
#include "RTMPChunk.hpp"
#include "RTMPSession.hpp"
#include "RTMPHandshake.hpp"
#include "RTMPMessage.hpp"

#include "../utils/Bit.hpp"
#include "../utils/amf0.hpp"

#include <iterator>


/**
 * win32 specific imports.
 **/
#ifdef _WIN32
#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#endif

namespace RTMP
{
    class Handler
    {
        public:
            /**
             * Handle incoming data.
             **/
            static int HandleCommandMessage(Netconnection::Command*, Session&);
            static void HandleVideoMessage(unsigned char*, Session&);
            static void HandleAudioMessage(unsigned char*, Session&);

            static int HandleChunk(Chunk& chunk, Session& session);

            /**
             * Send data.
             **/
            static int SendData(SOCKET socket, char* data, int length);
            static int SendChunk(char* data, int length, Session& session);

            static int SendCommandMessage(Netconnection::Command*, Session&);
            static int SendHandshake(Session&);
            static void SendVideoMessage(unsigned char*, Session&);
            static void SendAudioMessage(unsigned char*, Session&);
    };
}