#pragma once
#include "rtp.hpp"
#include <stdio.h>

#define DEFAULT_BUFLEN 4096

struct RTMPPacket{
    RTCP::PACKET_TYPE packet_type;
    RTCP::SDES_TYPES sdes_type;
    struct RTCP::HEADER_TYPE* header_type;
    struct RTCP::COMMON_TYPE* common_type;
    struct RTCP::RR_TYPE* rr_type;
    struct RTCP::SDES_ITEM_TYPE* sdes_item_type;
    struct RTCP::TYPE* type;
    struct RTCP::source* source;
    
    
};

class RTMPController {
    public:
    char recvbuf[DEFAULT_BUFLEN];

    /** 
     * Parse received bytes into rtcp packet
     */
    static RTMPPacket& parseRequest(char b[]){
        RTMPPacket packet;

        printf("\n4 first Bytes: %x %x %x %x \n", b[0], b[1], b[2], b[3]);
        RTCP::HEADER_TYPE header_type = 
            RTCP::HEADER_TYPE{

            };
        RTCP::COMMON_TYPE common_type = 
            RTCP::COMMON_TYPE{
                (unsigned int)b[0],
                (unsigned int)b[1],
                (unsigned int)b[2], 
                (unsigned int)b[3], 
                sizeof(b)
            };
        RTCP::RR_TYPE rr_type = 
            RTCP::RR_TYPE{

            };
        RTCP::SDES_ITEM_TYPE sdes_item_type = 
            RTCP::SDES_ITEM_TYPE{
                
            };
        RTCP::TYPE type = 
            RTCP::TYPE{
                
            };

        packet.packet_type      = RTCP::PACKET_TYPE::APP;
        packet.sdes_type        = RTCP::SDES_TYPES::END;
        packet.header_type      = (struct RTCP::HEADER_TYPE*)&header_type;
        packet.common_type      = (struct RTCP::COMMON_TYPE*)&common_type;
        packet.rr_type          = (struct RTCP::RR_TYPE*)&rr_type;
        packet.sdes_item_type   = (struct RTCP::SDES_ITEM_TYPE*)&sdes_item_type;
        packet.type             = (struct RTCP::TYPE*)&type;

        return packet;
    }

    static void handleRequest(RTMPPacket& packet);      // 2
};