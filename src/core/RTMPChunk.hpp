#pragma once
/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-21
 **/

namespace RTMP
{

    namespace ChunkHeader
    {
        /**
         * The Chunk Basic Header encodes the chunk stream id (csid) 
         * and the chunk type (fmt). Chunk type determines the format
         * of the encoded message header. Chunk Basic Header field may
         * be 1, 2 or 3 bytes depending on the chunk stream ID (csid).
         **/
        struct BasicHeader
        {
            /**
             * fmt (2 bits): This field identifies one of the four format used by the
             * chunk message header. The chunk message header for each of the chunk
             * types is explained in the next section.
             **/ 
            unsigned int fmt;

            /**
             * 
             * cs id (6 bits): This field contains the chunk stream ID, for values
             * from 2-63. Values 0 and 1 are used to indicate the 2 or 3 bytes
             * versions of this field.
             * Values: 3-65599
             * 
             * ID:
             * Value 0 -> 64-319:       2th byte + 64
             * Value 1 -> 64- 65599:    (3rd byte * 256) + 2th byte + 64
             * Value 2 -> Reserved for low-level protocol control messages & commands.
             * Values in the range of 3-63 are the complete stream ID.
             * 
             * Chunk stream IDs 2-63 can be encoded in the 1 byte version of the field. 
             **/
            unsigned int csid;

            /** 
             * cs id - 64 (8 or 16 bits): This field contains the chunk stream ID minus
             * 64. For example, ID 365 would be represented by a 1 in cs id, and a 16-bit
             * 301 here.
             **/
            unsigned int csid_minus_64;
        };

        struct MessageHeader
        {

        };
    };

    /**
     * The chunks are transmitted over the network. While transmitting,
     * each chunk must be sent in full before the next chunk.
     **/
    struct Chunk
    {
        /**
         * Basic Header
         * Size: 1 to 3 bytes.
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
