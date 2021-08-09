#pragma once

class NetStream
{
    public:
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
        void play();
        void play2();
        void deleteStream();
        void closeStream();
        void receiveAudio();
        void receiveVideo();
        void publish();
        void seek();
        void pause();

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
        void onStatus();
};