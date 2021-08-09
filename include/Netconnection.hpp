#pragma once
/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-08
 * Documentation: 
 *  - https://www.adobe.com/content/dam/acom/en/devnet/rtmp/pdf/rtmp_specification_1.0.pdf
 *  (Page 29-33)
 **/

#include <string>
#include <map>

struct ServerCommand
{
    std::string name;
    unsigned int transactionId;
    struct Properties
    {

    };
    struct Information
    {

    };
};

struct ClientCommand
{
    std::string name;
    unsigned int transactionId:1;
    char* optionalUserArguments;
    struct CommandObject
    {
        std::string app;
        std::string flashver;
        std::string swfUrl;
        std::string tcUrl;
        std::string pageUrl;
        bool tcUrl;
        Netconnection::AudioCodecs audioCodecs;
        Netconnection::VideoCodecs videoCodecs;
        Netconnection::ObjectEncoding objectEncoding;

    };
};

class Netconnection
{
    public:
        /**
         * Connect command:
         * 
         * 1- Client sends the connect command to the server to
         *    request to connect with the server application instance.
         * 2- After receiving the connect command, the server sends
         *    the protocol message 'Window Acknowledgement Size' to 
         *    the client. The server also connects the client to the app.
         * 3- The server sends the protocol message 'Set Peer Bandwidth'
         *    to the client.
         * 4- The client sends the protocol message 'Window Ack. Size'
         *    to the server after processing the protocol message
         *    'Set Peer Bandwidth'.
         * 5- The server sends another protocol message of type
         *    User Control Message(StreamBegin) to the client.
         * 6- The server sends the result command message informing the
         *    client of the connection status (success/fail). The command
         *    specifies the transaction ID (always equal to 1 for the connect
         *    command). The message also specifies the properties, such as
         *    Flash Media Server version (string). In addition, it specifies
         *    other connection response related infromations like level (string),
         *    code (string), description (string), ObjectEncoding (number)...
         **/
        void connect();
        /**
         * The call method runs remote procedure call (RPC) at the
         * receiving end.
         **/
        void call(std::string RPCName);
        void close();
        /**
         * The client sends this command to the server to create a
         * logical channel for message communication. The publishing
         * of audio, video and metadata is carried out over stream
         * channel created using the createStream command.
         * 
         * NetConnection is the default communication channel, which
         * has a stream ID of 0. Protocol and a few command messages,
         * including createStream, use the default communication channel.
         **/
        void createStream();

        enum AudioCodecs
        {
            /**
             * Raw sound, no compression.
             **/
            SUPPORT_SND_NONE = 0x0001,
            /**
             * ADPCM compression.
             **/
            SUPPORT_SND_ADPCM = 0x0002,
            /**
             * MP3 compression.
             **/
            SUPPORT_SND_MP3 = 0x0004,
            /**
             * Not used.
             **/
            SUPPORT_SND_INTEL = 0x0008,
            /**
             * Not used.
             **/
            SUPPORT_SND_UNUSED = 0x0010,
            /**
             * NellyMoser at 8 kHz compression.
             **/
            SUPPORT_SND_NELLY8 = 0x0020,
            /**
             * NellyMoser compression (5, 11, 22 and 44 kHz).
             **/
            SUPPORT_SND_NELLY = 0x0040,
            /**
             * G711A sound compression (Flash Media Server only).
             **/
            SUPPORT_SND_G711A = 0x0080,
            /**
             * G711U sound compression (Flash Media Server only).
             **/
            SUPPORT_SND_G711U = 0x0100,
            /**
             * NellyMouser at 16 kHz compression.
             **/
            SUPPORT_SND_NELLY16 = 0x0200,
            /**
             * Advanced audio coding (AAC) codec.
             **/
            SUPPORT_SND_AAC = 0x0400,
            /**
             * Speex audio
             **/
            SUPPORT_SND_SPEEX = 0x0800,
            /**
             * All RTMP-supported audio codecs.
             **/
            SUPPORT_SND_ALL = 0x0FFF
        };

        enum VideoCodecs
        {
            /**
             * Obsolete value.
             **/
            SUPPORT_VID_UNUSED = 0x0001,
            /**
             * Obsolete value.
             **/
            SUPPORT_VID_JPEG = 0x0002,
            /**
             * Sorenson Flash video.
             **/
            SUPPORT_VID_SORENSON = 0x0004,
            /**
             * V1 screen sharing.
             **/
            SUPPORT_VID_HOMEBREW = 0x0008,
            /**
             * On2 video (Flash 8+).
             **/
            SUPPORT_VID_VP6 = 0x0010,
            /**
             * On2 video with alpha channel.
             **/
            SUPPORT_VID_VP6ALPHA = 0x0020,
            /**
             * Screen sharing version 2 (Flash 8+).
             **/
            SUPPORT_VID_HOMEBREWV = 0x0040,
            /**
             * H264 video.
             **/
            SUPPORT_VID_H264 = 0x0080,
            /**
             * All RTMP-supported video codecs.
             **/
            SUPPORT_VID_ALL = 0x00FF
        };

        enum VideoFunction 
        {
            /**
             * Indicates that the client can perform frame-accurate seeks.
             **/
            SUPPORT_VID_CLIENT_SEEK = 1
        };

        enum ObjectEncoding
        {
            AMF0 = 0,
            AMF3 = 3
        };
};