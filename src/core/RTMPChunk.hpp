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
            //unsigned int csid_minus_64;

            unsigned int baseID;
        };

        /** 
         * There are four different formats for the chunk message header,
         * selected by the "fmt" field in the chunk basic header.
         **/
        struct MessageHeader
        {
            enum ChunkHeaderFormat
            {
                /**
                 * Size: 11 bytes.
                 * 
                 * This type must be used at the start fo a chunk stream, and whenever
                 * the stream timestamp goes backward (e.g., because of a backward seek).
                 **/
                Type0,

                /**
                 * Size: 7 bytes.
                 * 
                 * The message type ID is not included; this chunk takes the same stream ID
                 * as the preceding chunk. Streams with variable-sized messages (for example,
                 * many video formats) should use this format for the first chunk of each new
                 * message after the first.
                 **/
                Type1,

                /**
                 * Size: 3 bytes.
                 * 
                 * Neither the stream ID nor the message length is included; this chunk has the 
                 * same stream ID and message length as the preceding chunk. Streams with constant
                 * sized messages (for example, some audio and data formats) should use this format
                 * for the first chunk of each message after the first.
                 **/
                Type2,

                /**
                 * Size: 0 bytes.
                 * 
                 * Type 3 chunks have no message header. The stream ID, message length and timestamp
                 * delta fields are not present; chunks of this type take values from the preceding
                 * chunk for the same Chunk Stream ID. When a single message is split into chunks,
                 * all chunks of a message except the first one should use this type.
                 * 
                 * A stream consisting of messages of exacly the same size, stream ID and spacing
                 * in time should use this type for all chunks after a chunk of type 2.
                 * 
                 * If the delta between the first message and the second message is same as the
                 * timestamp of the first message, then a chunk of Type 3 could immidiately follow
                 * the chunk of Type 0 as there is no need for a chunk of Type 2 to register the delta.
                 * If a Type 3 chunk follows a Type 0 chunk, then the timestamp delta for this Type 3
                 * chunk is the same as the timestamp of the Type 0 chunk.
                 **/
                Type3
            };
            
            int displacement = 0;

            /**
             * Size: 3 bytes.
             * 
             * For a Type 1 or Type 2 chunk, the difference between the previous chunk's timestamp
             * and the current is sent here. If the delta is greater than or equal to 16777215 (0xFFFFFF),
             * this field MUST be 0xFFFFFF, indicating the presence of the Extended Timestamp field to
             * encode the full 32 bit delta. Otherwise, this field SHOULD be the actual delta.
             **/
            int timestamp_delta;

            /**
             * Size: 3 bytes.
             * 
             * For a Type 0 or Type 1 chunk, the length of the message is sent here. Note that this is
             * generally not the same as the length of the chunk payload. The chunk payload length
             * remainer (which may be the entire length, for a small message) for the last chunk.
             **/
            int message_length;

            /**
             * Size: 1 byte.
             * 
             * For a Type 0 or Type 1 chunk, type of the message is sent here.
             **/
            int message_type_id;

            /**
             * Size: 4 bytes.
             * 
             * For a Type 0 chunk, the message  stream ID is stored. Message stream ID is stored
             * in little-endian format. Typically, all messages in the same chunk stream will come from
             * the same message stream. While it is possible to multiplex separate message streams into
             * the same chunk stream, this defeats the benefits of the header compression. However, if
             * one message stream is closed and another one subsequently opened, there is no reason an
             * existing chunk stream cannot be reused by sending a new Type 0 chunk.
             **/
            int message_stream_id;
        };
    };

    /**
     * The chunks are transmitted over the network. While transmitting,
     * each chunk must be sent in full before the next chunk.
     **/
    struct Chunk
    {
        int displacement = 0;

        /**
         * Basic Header
         * Size: 1 to 3 bytes.
         * 
         * This field encodes the chunk stream ID and the chunk type. Chunk type
         * determines the format of the encoded message header. The length depends
         * entirely on the chunk stream ID, which is a variable-length field.
         **/
        ChunkHeader::BasicHeader basicHeader;

        /**
         * Message Header
         * Size: 0, 3, 7 or 11 bytes.
         * 
         * This field encoded information about the message being sent (whether in
         * whole or in part). The length can be determined using the chunk type
         * specified in the chunk header.
         **/
        ChunkHeader::MessageHeader messageHeader;

        /**
         * Extended Timestamp
         * Size: 0 or 4 bytes.
         * 
         * This field is present in certain circumstances depending on the encoded 
         * timestamp or timestamp delta field in the chunk message header.
         * 
         * The Extended Timestamp field is used to encode timestamps or timestamp deltas
         * that are greater than 0xFFFFFF; that is, for timestamps or timestamp deltas
         * that don't fit in the 24-bit fields of Type 0, 1 or 2 chunks. This field encodes
         * the complete 32-bit timestamp or timestamp delta. The presence of this field is
         * indicated by setting the timestamp field of a Type1 or 2 chunk, to 0xFFFFFF.
         * This field is present in Type 3 chunks when the most recent Type 0, 1 or 2 chunk
         * for the same chunk stream ID indicated the presence of an extended timestamp field.
         **/
        int extendedTimestamp;

        /**
         * Chunk Data
         * Size: Varies.
         * 
         * The payload of this chunk, up to the configured maximum chunk size.
         **/
        unsigned char* data = nullptr;

        int missingData = 0;

    };
}
