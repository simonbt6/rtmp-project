#include "StreamDecoder.hpp"

namespace Codecs
{
    StreamDecoder::StreamDecoder(AVCodecID decoder_id)
    {
        m_codec = avcodec_find_decoder(decoder_id);
        m_codec_ctx = avcodec_alloc_context3(m_codec);
        m_parser_ctx = av_parser_init(m_codec->id);
        avcodec_open2(m_codec_ctx, m_codec, 0);
        m_frame = av_frame_alloc();

    }

    StreamDecoder::~StreamDecoder()
    {
        av_free(m_frame);
        avcodec_close(m_codec_ctx);
        av_parser_close(m_parser_ctx);
    }

    void StreamDecoder::Decode(AVCodecContext * decode_ctx, AVFrame* frame, AVPacket* packet, const char* filename)
    {
        char buf[1024];
        int ret;

        ret = avcodec_send_packet(decode_ctx, packet);

        if (ret < 0)
            return Utils::FormatedPrint::PrintError("VideoDecoder::decode", "Error sending a packet for decoding.");
        while (ret >= 0)
        {
            ret = avcodec_receive_frame(decode_ctx, frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                return Utils::FormatedPrint::PrintError("VideoDecoder::decode", "EAGAIN OR AVERROR_EOF");
            else if (ret < 0)
                return Utils::FormatedPrint::PrintError("VideoDecoder::decode", "Error during decoding.");
            
            Utils::FormatedPrint::PrintFormated("VideoDecoder::decode", "Saving frame " + std::to_string(decode_ctx->frame_number));
            fflush(stdout);

            string filename = "_frame_" + std::to_string(decode_ctx->frame_number);

            StreamDecoder::save_gray_frame(frame->data[0], frame->linesize[0], frame->width, frame->height, filename.c_str());
        }
    }

    AVPacket* StreamDecoder::ParsePacket(uint8_t* data, size_t size)
    {
        const char* out_filename;

        AVPacket* packet = new AVPacket();
        av_init_packet(packet);

        int response = 0;

        int ret;

        while (size > 0)
        {
            ret = av_parser_parse2(m_parser_ctx, m_codec_ctx, &packet->data, &packet->size, data, size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);

            if (ret < 0)
                Utils::FormatedPrint::PrintError("StreamDecoder::ParsePacket", "Error while parsing.");
            data += ret;
            size -= ret;

            if (packet->size)
                Decode(m_codec_ctx, m_frame, packet, out_filename);
        }        

        Decode(m_codec_ctx, m_frame, NULL, out_filename);


        // char frame_filename[1024];
        // snprintf(frame_filename, sizeof(frame_filename), "%s-%d.pgm", "frame", m_codec_ctx->frame_number);

        // if (m_frame->format != AV_PIX_FMT_YUV420P)
        //     Utils::FormatedPrint::PrintInfo("Warning: The generated file may not be a grayscale image, but could e.g. be just the R component if the video format is RGB.");
            
        // save_gray_frame(m_frame->data[0], m_frame->linesize[0], m_frame->width, m_frame->height, frame_filename);

        return packet;
    }

    void StreamDecoder::save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, const char *filename)
    {
        FILE *f;
        int i;
        f = fopen(filename,"w");
        // writing the minimal required header for a pgm file format
        // portable graymap format -> https://en.wikipedia.org/wiki/Netpbm_format#PGM_example
        fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

        // writing line by line
        for (i = 0; i < ysize; i++)
            fwrite(buf + i * wrap, 1, xsize, f);
        fclose(f);
    }

    void StreamDecoder::save_to_file(unsigned char* data, int size, int i)
    {
        const char* filename = "packet_" + (48 + i);
        FILE* f = fopen(filename, "w");

        for (int n = 0; n < size; n += 16)
        {
            if (n % 16 == 0 && n != 0) fprintf(f, "\n");
            if (n % 8 == 0 && n != 0) fprintf(f, "   ");
            fprintf(f, "%X ", data[n]);

            if (data[n] < 0x10) fprintf(f, " ");
        }
    }
};