#pragma once
/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-21
 **/

namespace RTMP
{
    /**
     * The chunks are transmitted over the network. While transmitting,
     * each chunk must be sent in full before the next chunk.
     **/
    struct Chunk
    {
        /**
         * Basic Header
         * Size: 1 or 3 bytes.
         * 
         * This field encodes the chunk stream ID and the chunk type. Chunk type
         * determines the format of the encoded message header. The length depends
         * entirely on the chunk stream ID, which is a variable-length field.
         **/
        int* BasicHeader;

        /**
         * Message Header
         * Size: 0, 3, 7 or 11 bytes.
         * 
         * This field encoded information about the message being sent (whether in
         * whole or in part). The length can be determined using the chunk type
         * specified in the chunk header.
         **/
        int* MessageHeader;

        /**
         * Extended Timestamp
         * Size: 0 or 4 bytes.
         * 
         * This field is present in certain circumstances depending on the encoded 
         * timestamp or timestamp delta field in the chunk message header.
         **/
        int* ExtendedTimestamp;

        /**
         * Chunk Data
         * Size: Varies.
         * 
         * The payload of this chunk, up to the configured maximum chunk size.
         **/
        int* Data;

    };
}
