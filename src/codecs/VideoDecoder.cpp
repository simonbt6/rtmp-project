#include "VideoDecoder.hpp"

namespace Codecs
{
    void VideoDecoder::ReadVideoFile(const std::string& filepath)
    {
        av_format_ctx = avformat_alloc_context();
        if (!av_format_ctx) throw std::runtime_error("\nFailed to create AVFormatContext.");

        if (avformat_open_input(&av_format_ctx, filepath.c_str(), NULL, NULL) != 0) 
            throw std::runtime_error("\nFailed to open video file.");

        video_stream_index = -1;
        AVCodecParameters* av_codec_params;
        AVCodec* av_codec;

        for (int i = 0; i < av_format_ctx->nb_streams; i++)
        {
            av_codec_params = av_format_ctx->streams[i]->codecpar;
            av_codec = avcodec_find_decoder(av_codec_params->codec_id);
            if (!av_codec) continue;
            if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO)
            {
                video_stream_index = i;
                width = av_codec_params->width;
                height = av_codec_params->height;
                time_base = av_format_ctx->streams[i]->time_base;
                break;
            } 
        }
        if (video_stream_index == -1) throw std::runtime_error("\nFailed to find a valid video stream inside file.");
        
        av_codec_ctx = avcodec_alloc_context3(av_codec);
        if (!av_codec_ctx) throw std::runtime_error("\nFailed to create AVCodecContext.");

        if (avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0) 
            throw std::runtime_error("\nFailed to initialize AVCodecContext.");
        
        if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0) throw std::runtime_error("\nFailed to open codec.");

        av_frame = av_frame_alloc();
        if (!av_frame) throw std::runtime_error("\nFailed to allocate AVFrame.");

        av_packet = av_packet_alloc();
        if (!av_packet) throw std::runtime_error("\nFailed to allocate AVPacket.");
    }

    void VideoDecoder::ReadFrame(uint8_t* frame_buffer, int64_t* pts)
    {
        int response;
        while (av_read_frame(av_format_ctx, av_packet) >= 0)
        {
            if (av_packet->stream_index != video_stream_index)
            {
                av_packet_unref(av_packet);
                continue;
            }

            response = avcodec_send_packet(av_codec_ctx, av_packet);
            if (response < 0) throw std::runtime_error("\nFailed to decode packet (send).");

            response = avcodec_receive_frame(av_codec_ctx, av_frame);
            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
            {
                av_packet_unref(av_packet);
                continue;
            } else if (response < 0) throw std::runtime_error("\nFailed to decode packet (receive).");

            av_packet_unref(av_packet);
            break;
        }
        *pts = av_frame->pts;

        if (!sws_scaler_ctx)
            sws_scaler_ctx = sws_getContext(width, height, av_codec_ctx->pix_fmt,
                                            width, height, AV_PIX_FMT_RGB0,
                                            SWS_BILINEAR, NULL, NULL, NULL);

        if (!sws_scaler_ctx) throw std::runtime_error("\nFailed to initialize sw scaler.");

        uint8_t* dest[4] = { frame_buffer, NULL, NULL, NULL };
        int dest_linesize[4] = { width * 4, 0, 0, 0 };
        sws_scale(sws_scaler_ctx, av_frame->data, av_frame->linesize, 0, av_frame->height, dest, dest_linesize);
    }

    void VideoDecoder::SeekFrame(int64_t ts)
    {
        av_seek_frame(av_format_ctx, video_stream_index, ts, AVSEEK_FLAG_BACKWARD);

        int response;
        while (av_read_frame(av_format_ctx, av_packet) >= 0)
        {
            if (av_packet->stream_index != video_stream_index)
            {
                av_packet_unref(av_packet);
                continue;
            }

            response = avcodec_send_packet(av_codec_ctx, av_packet);
            if (response < 0) throw std::runtime_error("\nFailed to decode packet.");

            response = avcodec_receive_frame(av_codec_ctx, av_frame);
            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
            {
                av_packet_unref(av_packet);
                continue;
            }
            else if (response < 0) throw std::runtime_error("\nFailed to decode packet.");

            av_packet_unref(av_packet);
            break;
        }
    }

    void VideoDecoder::Close()
    {
        sws_freeContext(sws_scaler_ctx);

        avformat_close_input(&av_format_ctx);
        avformat_free_context(av_format_ctx);
        
        avcodec_free_context(&av_codec_ctx);
        
        av_frame_free(&av_frame);
        
        av_packet_free(&av_packet);
    }
}