#pragma once

/**
 * Author: Simon Brisebois-Therrien
 * Date: 2021-08-09
 * 
 * Documentation: 
 *  - https://help.adobe.com/en_US/FlashPlatform/reference/actionscript/3/flash/net/NetStreamPlayOptions.html
 **/

#include "Netconnection.hpp"
#include "RTMPSession.hpp"

using namespace RTMP;

class NetStream
{
    public:
        enum class PlayStatus
        {
            Uninitialized,
            Pause,
            Resumed,
            Stopped,
        };
        /** 
         * Client commands.
         **/
        /** 
         * The client sends this command to the server to play a stream. 
         * A playlist can also be created using this command multiple times.
         * 
         * If you want to create a dynamic playlist that switches among
         * different live or recorded streams, call play more than once
         * and pass false for reset each time. Conversely, if you want 
         * to play the specified stream immediately, clearing any other
         * streams that are queued for play, pass true for reset.
         * 
         * Command structure: 
         *  - Command name: Name of the command. Set to "play".
         *  - Transaction ID: 0
         *  - Command object: null
         *  - Stream Name: Name of the stream to play.
         *  - Start: Optional param. which specifies the start time in seconds.
         *  - Duration: Optional param. that specifies the duration of the playback in seconds.
         *  - Reset: Bool. Whether to flush any previous playlist.
         **/
        void play(Netconnection::Play&, RTMP::Session&);
        /**
         * Play2 can switch to a different bit rate stream without changing
         * the timeline of the content played.
         * 
         * Command structure: 
         *  - Command name: Name of the command. Set to "play2".
         *  - Transaction ID: 0
         *  - Command object: null
         *  - Parameters: AMF encoded object whose properties are the public properties
         *                described for the flash.net.NetStreamPlayOptions ActionScript object.
         *                (See documentation.)
         **/
        void play2(Netconnection::Play2&, RTMP::Session&);
        /**
         * NetStream sends the deleteStream command when the NetStream object is getting destroyed.
         * 
         * Command structure: 
         *  - Command name: Name of the command. Set to "deleteStream".
         *  - Transaction ID: 0
         *  - Command object: null.
         *  - Stream ID: The ID of the stream that is destroyed on the server.
         * 
         * Server doesn't send any response.
         **/
        void deleteStream(Netconnection::DeleteStream&, RTMP::Session&);
        void closeStream(); //???
        void receiveAudio(Netconnection::ReceiveAudio&, RTMP::Session&);
        void receiveVideo(Netconnection::ReceiveVideo&, RTMP::Session&);
        void publish(Netconnection::Publish&, RTMP::Session&);
        void seek(Netconnection::Seek&, RTMP::Session&);
        void pause(Netconnection::Pause&, RTMP::Session&);

        /** 
         * Server commands.
         **/
        /** 
         * Command name: onStatus
         * Transaction ID: 0
         * Command object: Null
         * Information object: 
         *  - Level: Level for this message.
         *      - Warning
         *      - Status
         *      - Error
         *  - Code: The message code.
         *      - ex: "NetStream.Play.Start"
         *  - Description: a human readable description of the message.
         **/
        void onStatus(Netconnection::OnStatus&, RTMP::Session&);
};