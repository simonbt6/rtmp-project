#pragma once
#include "rtp.hpp"
#include <stdio.h>

#define DEFAULT_BUFLEN 4096

struct RTMPPacket{
    RTCP::PACKET_TYPE packet_type;
    RTCP::SDES_TYPES sdes_type;
    struct RTCP::HEADER_TYPE header_type;
    struct RTCP::COMMON_TYPE common_type;
    struct RTCP::RR_TYPE rr_type;
    struct RTCP::SDES_ITEM_TYPE sdes_item_type;
    struct RTCP::TYPE type;
    struct RTCP::source source;
    
    
};

class RTMPController {
    char recvbuf[DEFAULT_BUFLEN];
    public:
    static RTMPPacket& parseRequest(char b[]){
        RTMPPacket packet;
        printf("\n4 first Bytes: %x %x %x %x \n", b[0], b[1], b[2], b[3]);
        packet.common_type = 
            RTCP::COMMON_TYPE{
                (unsigned int)b[0],
                (unsigned int)b[1],
                (unsigned int)b[2], 
                (unsigned int)b[3], 
                sizeof(b)
            };
        packet.header_type = 
            RTCP::HEADER_TYPE{

            };
        packet.packet_type = 
            RTCP::PACKET_TYPE{

            };
        return packet;
    }           // 1 Parse received bytes into rtcp packet

    static void handleRequest(RTMPPacket& packet);      // 2
};