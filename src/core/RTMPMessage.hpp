#pragma once
/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-08
 **/

#include "rtp.hpp"

namespace RTMP
{

    struct ProtocolControlMessage
    {
        enum Type
        {
            SetChunkSize = 0x01,
            
            Abort = 0x02,

            Acknowledgement = 0x03,
            
            WindowAcknowledgementSize = 0x05,

            SetPeerBandwidth = 0x06,
        };

    };

    struct Message {

        enum Type {
            
            /**
             * The client or the server sends this message to send audio
             * data to the peer.
             **/
            AudioMessage = 0x08,
            /**
             * The client or the server sends this message to send video
             * data to the peer.
             **/             
            VideoMessage = 0x09,
            /**
             * An aggregate message is a single message that contains a 
             * series of RTMP sub-messages using the format described in 6.1.
             **/             
            AggregateMessage = 0x16,
            /**
             * The client or the server sends this message to notify the peer
             * about the user control events. Section 6.2.
             **/

            //UserControlMessageEvents,

            /**
             * AMF0 Encoding
             **/
            AMF0CommandMessage = 0x14,
            AMF0DataMessage = 0x12,
            AMF0SharedObjectMessage = 0x13,
            
            /**
             * AMF3 Encoding
             **/
            AMF3DataMessage = 0x0D,
            AMF3SharedObjectMessage = 0x0E,
            AMF3CommandMessage = 0x0F,
        };

        enum UserControlMessageEventsTypes
        {
            /**
             * The server sends this event to notify the client
             * that a stream has become functional and can be
             * used for communication.
             * Size: 4 bytes
             **/
            StreamBegin = 0x00,
            /** 
             * The server sends this event to notify the client
             * that the playback of data is over as requested
             * on this stream. No more data is sent without issuing
             * additional commands. The client discards the message
             * received from the stream.
             * Size: 4 bytes
             **/
            StreamEOF = 0x01,
            /**
             * The server sends this event to notify the client
             * that there is no more data on the stream. If the server
             * does not detect any message for a time period, it can
             * notify the subscribed clients that the stream is dry.
             * Size: 4 bytes
             **/
            StreamDry = 0x02,
            /**
             * The client sends this event to inform the server
             * of the buffer size (in ms) that is used to buffer 
             * any data coming over a stream. This event is sent
             * before the server starts processing the stream.
             * Size: 8 bytes
             *  - 4 bytes: stream id
             *  - 4 bytes: buffer length
             * 
             **/
            SetBufferLength = 0x03,
            /**
             * The server sends this event to notify the client
             * that the stream is a recorded stream.
             **/
            StreamIsRecorded = 0x04,
            /**
             * The server sends this event to test whether the client
             * is reachable. The client responds with PingResponse.
             * Size: 4 bytes
             *  - 4 bytes: timestamp
             *      - Local server time.
             **/
            PingRequest = 0x06,
            /**
             * The client sends this event to the server in response
             * to PingRequest.
             * Size: 4 bytes
             *  - 4 bytes: timestamp
             *      - Received timestamp from PingRequest.
             **/
            PingResponse = 0x07
        };

        enum EventType {
            /**
             * The client sends this event to inform the server about
             * the creation of a named shared object.
             **/
            Use = 0x01,
            /**
             * The client sends this event to inform the server when
             * the shared object is deleted on the client side.
             **/
            Release = 0x02,
            /**
             * The client sends this event to request that the change
             * of value associated with a named parameter of the shared object.
             **/
            RequestChange = 0x03,
            /**
             * The server sends this event to notify all clients, except
             * the client originating the request, of a change in the value
             * of a named parameter.
             **/
            Change = 0x04,
            /**
             * The server sends this event to the requesting client in response
             * to RequestChange event if the request is accepted.
             **/
            Success = 0x05,
            /**
             * The client sends this event to the server to broadcast a
             * message. On receiving this event, the server broadcasts a message
             * to all the clients, including the sender.
             **/
            SendMessage = 0x06,
            /**
             * The server sends this event to notify clients about error conditions.
             **/
            Status = 0x07,
            /**
             * The server sends this event to the client to clear a shared object.
             * The server also sends this event in response to Use event that the
             * client sends on connect.
             **/
            Clear = 0x08,
            /**
             * The server sends this event to have the client delete a slot.
             **/
            Remove = 0x09,
            /**
             * The client sends this event to have the client delete a slot.
             **/
            RequestRemove = 0x0A,
            /**
             * The server sends this event to the client on a successful connection.
             **/
            UseSuccess = 0x0B,
        };

        struct Format {};
        
        /** 
         * Message header.
         **/
        struct Header {
            /** 
             * Message type
             * Size: 1 byte
             * Description: Field to represent the message type.
             **/
            unsigned int type;
            /**
             * Payload size
             * Size: 3 bytes
             * Description: Represents the size of the payload in bytes.
             * Format: Big-endian
             **/
            unsigned int length;
            /**
             * Timestamp
             * Size: 4 bytes
             * Description: Timestamp of the message.
             * Format: Big-endian
             **/
            unsigned int timestamp;
            /**
             * Message stream id
             * Size: 3 bytes
             * Description: Identifies the stream of the message.
             * Format: Big-endian
             **/
            unsigned int streamid;
        };

        /**
         * Message payload
         * Description: The message payload is the data
         * contained in the message. It could be either audio or video samples.
         **/
        int* Payload;
    };
}
