#include <sys/types.h>

#define RTP_VERSION 2           // Protocol version

#define RTP_SEQ_MOD (1<<16)
#define RTP_MAX_SDES 255        // Maximum text length for SDES

typedef unsigned char   u_int8;
typedef unsigned short  u_int16;
typedef unsigned int    u_int32;

namespace RTCP{
    enum PACKET_TYPE{
        SR = 200,               // Sender report 200
        RR = 201,               // Receiver report
        SDES = 202,             // Source description
        BYE = 203,              // Goodbye
        APP = 204,              // Application-defined
    };
    
    enum SDES_TYPES{
        END = 0,                // End of SDES list
        CNAME = 1,              // Canonical name
        NAME = 2,               // User name
        EMAIL = 3,              // User's electronic mail address
        PHONE = 4,              // User's phone number
        LOC = 5,                // Geographic user location
        TOOL = 6,               // Name of application or tool
        NOTE = 7,               // Notice about the source
        PRIV = 8,               // Private extension
    };

    /**
     *  RTP data header 
     **/
    struct HEADER_TYPE{
        unsigned int version:2; // Protocol version
        unsigned int p:1;       // Padding flag
        unsigned int x:1;       // Header extension flag
        unsigned int cc:4;      // CSRC count
        unsigned int m:1;       // Marker bit
        unsigned int pt:7;      // Payload type
        unsigned int seq:16;    // Sequence number
        u_int32 ts;             // Timestamp
        u_int32 ssrc;           // Synchronization source
        u_int32 csrc[1];        // Optional CSRC list
    };

    /**
     * RTCP common header word
     **/
    struct COMMON_TYPE{
        unsigned int version:2; // Protocol version
        unsigned int p:1;       // Padding flag
        unsigned int count:5;   // Varies by packet type
        unsigned int pt:8;      // RTCP packet type
        u_int16 length;         // Packet length in words
    };

    /**
     * Reception report block
     **/
    struct RR_TYPE{
        u_int32 ssrc;           // Data source being reported
        unsigned int fraction:8;// Fraction lost since last SR/RR
        int lost:24;            // Cummulative number packet lost (signed)
        u_int32 last_seq;       // Extended last sequence number received
        u_int32 jitter;         // Interarrival jitter
        u_int32 lsr;            // Last SR packet from this source
        u_int32 dlsr;           // Delay since last SR packet
    };

    /**
     *  SDES item 
     **/
    struct SDES_ITEM_TYPE{
        u_int8 type;            // Type of item (SDES_TYPE)
        u_int8 length;          // Length of item in octets
        char data[1];           // Text, not null-terminated
    };

    /**
     * One RTCP packet
     **/
    struct TYPE{
        COMMON_TYPE common;
        union {
            // Sender report
            struct SR {
                u_int32 ssrc;   // Sender generating this report
                u_int32 ntp_sec;// NTP timestamp
                u_int32 ntp_frac;
                u_int32 rtp_ts; // RTP timestamp
                u_int32 psent;  // Octets sent
                RR_TYPE rr[1];  // Variable-length list
            };

            // Reception report
            struct RR {
                u_int32 ssrc;   // Receiver generating this report
                RR_TYPE rr[1];  // Variable-length list
            };

            // Source description
            struct SDES {
                u_int32 src;    // First SSRC/CSRC
                SDES_ITEM_TYPE item [1]; // List of SDES items
            };

            // BYE
            struct BYE {
                u_int32 src[1]; // List of sources
            };
        } R;
    };

    typedef struct SDES SDES_TYPE;

    /**
     * Per-source state information
     **/
    struct source {
        u_int16 max_seq;        // Highest sequence of number seen
        u_int32 cycles;         // Shifted count of sequence number cycles
        u_int32 base_seq;       // Base sequence number
        u_int32 bad_seq;        // Last bad sequence number + 1
        u_int32 probation;      // Sequence packets till source is valid
        u_int32 received;       // Packets received
        u_int32 expected_prior; // Packets expected at last interval
        u_int32 received_prior; // Packets received at last interval
        u_int32 transit;        // Relative trans. time for previous packet
        u_int32 jitter;         // Estimated jitter
    };
};

#define RTCP_VALID_MASK (0xc000 | 0x2000 | 0xfe)
#define RTCP_VALID_VALUE ((RTP_VERSION << 14) | RTCP::PACKET_TYPE::SR)