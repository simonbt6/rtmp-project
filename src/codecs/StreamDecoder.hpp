#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-09-27 
 * 
 * 
 */

extern "C" 
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <inttypes.h>
};

#include <iostream>
#include <fstream>
#include <vector>
#include <FormatedPrint.hpp>

namespace Codecs
{
    class StreamDecoder
    {
        private:
            AVCodecContext* m_codec_ctx;
            AVCodecParserContext* m_parser_ctx;
            AVCodec* m_codec;
            AVFrame* m_frame;
            

        public:
            StreamDecoder(AVCodecID decoder_id);
            ~StreamDecoder();

            AVPacket* ParsePacket(uint8_t* data, size_t size);
            static void Decode(AVCodecContext * decode_ctx, AVFrame* frame, AVPacket* packet, const char* filename);

        private:
            static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, const char *filename);

            static void save_to_file(unsigned char* data, int size, int i);

    };
};