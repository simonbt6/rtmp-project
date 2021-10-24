#pragma once
extern "C" 
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <inttypes.h>
    #include <libswscale/swscale.h>
};

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-09-22 
 * 
 * 
 */

#include <iostream>
#include <vector>
#include <FormatedPrint.hpp>

namespace Codecs
{
    class VideoDecoder
    {
        private:
            /** 
             * Class members.
             **/
        public:
            VideoDecoder();

            struct Frame
            {
                uint8_t* data;
                uint32_t width;
                uint32_t height;
                inline uint32_t GetSize() const { return width * height * 4; }
            };

            void DecodeVideoData(std::string filename, std::vector<Frame*>* frames_data);

            void DecodePacket(AVPacket* packet, AVCodecContext* av_codec_ctx, AVFrame* frame, std::vector<Frame*>* frames_data);

        private:
            static void LoadFrame();
            static void LoadPacket();

            static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);

            static void SaveFrameAsJPEG(AVCodecContext* p_CodecCtx, AVFrame* p_Frame, int FrameNo);
            static Frame* GetFrameData(AVCodecContext* p_CodecContext, AVFrame* p_Frame);


            static void save_to_file(unsigned char* data, int size, int i);


    };
}