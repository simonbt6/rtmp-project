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
    // C0 & S0
    struct F0 
    {
        unsigned short int version:3;
    };

    // C1 & S1
    struct F1 
    {
        unsigned int time[TIME_BYTES_COUNT];
        unsigned char randomBytes[RANDOM_BYTES_COUNT];
    };

    // C2 & S2
    struct F2
    {
        unsigned int time[TIME_BYTES_COUNT];
        unsigned int time2[TIME_BYTES_COUNT];
        unsigned char randomBytes[RANDOM_BYTES_COUNT];
    };
}