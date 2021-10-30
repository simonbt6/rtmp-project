#pragma once
/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-21 
 * 
 * 
 */

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

#include <iostream>
#include <string>

namespace Codecs
{
    class VideoDecoder
    {
        public:
            AVRational time_base;

            AVFormatContext* av_format_ctx;
            AVCodecContext*  av_codec_ctx;

            int video_stream_index;

            AVFrame* av_frame;
            AVPacket* av_packet;
            SwsContext* sws_scaler_ctx;

        public:
            int width, height;

            void ReadVideoFile(const std::string& filepath);

            void ReadFrame(uint8_t* frame_buffer, int64_t* pts);

            void SeekFrame(int64_t ts);

            void Close();
            
    };
};