/**
 * Author: Simon Brisebois-Therrien
 **/
#pragma once

#define RANDOM_BYTES_COUNT 1528
#define TIME_BYTES_COUNT 4

/**
 * Definition of Handshake formats.
 **/
namespace Handshake
{
    enum State
    {
        /** 
         * The protocol version is sent during this stage. 
         * Both the client and server are uninitialized.
         * The client sends the protocol version in C0.
         * If the server supports the version, it sends S0
         * and S1 in response.
         **/
        Uninitialized = 0,
        VersionSent = 1,
        AcknowledgeSent = 2,
        DoneSent = 3
    };
    
    // C0 & S0
    struct F0 
    {
        unsigned short int version:3;
    };

    // C1 & S1
    struct F1 
    {
        unsigned int time[TIME_BYTES_COUNT];
        unsigned int randomBytes[RANDOM_BYTES_COUNT];
    };

    // C2 & S2
    struct F2
    {
        unsigned int time[TIME_BYTES_COUNT];
        unsigned int time2[TIME_BYTES_COUNT];
        unsigned int randomBytes[RANDOM_BYTES_COUNT];
    };

    struct Handshake
    {
        // Status
        State state = State::Uninitialized;

        // 
        F0 C0;
        F0& S0 = C0;

        //
        F1 C1;
        F1 S1;

        //
        F2 C2;
        F2 S2;
        
    };
}
