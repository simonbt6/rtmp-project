
#define _HAS_STD_BYTE 0
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <fstream>

#include "../src/core/RTMPHandshake.hpp"
#include "../src/core/RTMPChunk.hpp"
#include "../src/core/RTMPParser.hpp"
#include "../src/core/RTMPSession.hpp"

#include "../src/utils/FormatedPrint.hpp"
#include "../src/utils/logs.hpp"


#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
#endif

#define PORT 1935
#define DEFAULT_BUFLEN 4096
#define DEFAULT_PORT "1935"

#define STANDBY -2

vector<unsigned char> SignedToUnsignedVector(vector<char>& in, int length)
{
    vector<unsigned char> out(length);
    for (int i = 0; i < length; i++)
        out.at(i) = in[i];
    printf("\nOut vector size: %i [%i]", (signed)out.size(), length);
    // in.resize(length);
    return out;
}

int main(int argc, char** argv) {

    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    vector<char> recvBuffer(DEFAULT_BUFLEN);
    vector<unsigned char> recvData;
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    int flags = 1;
    setsockopt(ListenSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&flags, sizeof(flags));
    if (iResult == SOCKET_ERROR)
    {
        printf("\nsetsockopt failed.");
    }
    printf("\nRTMP server listening on port 1935.");

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    RTMP::Session session;
    session.socket = ClientSocket;
    int lastSize = 0;
    do {
        int expectedlength = 0;
        if (session.handshake.state == Handshake::State::Uninitialized)
            expectedlength = 1537;
        else if (session.handshake.state == Handshake::State::AcknowledgeSent)
            expectedlength = 1536;
        else if (session.lastChunk != nullptr && session.lastChunk->missingData)
            expectedlength = session.lastChunk->missingData;
        else expectedlength = DEFAULT_BUFLEN;

        iResult = recv(ClientSocket, &recvBuffer[0], expectedlength, 0);
        if (iResult > 0) {

            printf("\n\n[Bytes received: %d]\n", iResult);

            vector<unsigned char> data = SignedToUnsignedVector(recvBuffer, iResult);
            Utils::LOGS::Log(data.data(), data.size());
            iSendResult = RTMP::Parser::ParseData(data, session);
            printf("\nSend status: %i", iSendResult);

            if (!iSendResult && iSendResult != STANDBY) 
            {
                iSendResult = send( ClientSocket, &recvBuffer[0], iResult, 0);
                printf("\nMirrored data.");
            }
            
            if (iSendResult == SOCKET_ERROR) {
                printf("\nsend failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("\nBytes sent: %d", iSendResult);
        }
        else if (iResult == 0)
            printf("\nConnection closing...");
        else  {
            printf("\nrecv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();
}