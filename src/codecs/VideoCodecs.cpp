#include "VideoCodecs.hpp"

namespace Codecs
{
    VideoDecoder::VideoDecoder()
    {
        printf("\nInitializing video decoder...");
    }

    void VideoDecoder::DecodeVideoData(std::string filename, const std::vector<uint8_t>* data)
    {
        AVFormatContext* av_format_ctx = avformat_alloc_context();
        if (!av_format_ctx) 
            throw std::runtime_error("Could not allocate memory for format context.");

        Utils::FormatedPrint::PrintFormated(
            "VideoDecoder::DecodeVideoData",
            "Loading video file [" + filename + "]."
        );

        if(avformat_open_input(&av_format_ctx, filename.c_str(), NULL, NULL))
            throw std::runtime_error("Could not open video file.");

        std::string videoFileInformationPrintString = "Video file informations:\n\tFormat [";
            videoFileInformationPrintString.append(av_format_ctx->iformat->name);
            videoFileInformationPrintString.append("]\n\tDuration [");
            videoFileInformationPrintString.append(std::to_string(av_format_ctx->duration));
            videoFileInformationPrintString.append("]\n\tBit rate [");
            videoFileInformationPrintString.append(std::to_string(av_format_ctx->bit_rate));
            videoFileInformationPrintString.append("]\n");

        Utils::FormatedPrint::PrintFormated(
            "VideoDecoder::DecodeVideoData",
            videoFileInformationPrintString);

        if (avformat_find_stream_info(av_format_ctx, NULL) < 0)
            throw std::runtime_error("Could not get the stream info.");
        
        AVCodec* p_codec;
        AVCodecParameters* p_codec_parameters = NULL;
        int video_stream_index = -1;

        for (int i = 0; i < av_format_ctx->nb_streams; i++)
        {
            AVCodecParameters* p_local_codec_parameters = NULL;
            AVStream* stream = av_format_ctx->streams[i];

            p_local_codec_parameters = stream->codecpar;
            string  av_stream_info = " [AVStream][" + std::to_string(i + 1) + "] Stream info:\n";
                    av_stream_info.append("\tTime base before open coded: " +  std::to_string(stream->time_base.num) + " / " + std::to_string(stream->time_base.den));
                    av_stream_info.append("\n\tFramerate before open coded: " + std::to_string(stream->r_frame_rate.num) + " / " + std::to_string(stream->r_frame_rate.den));
                    av_stream_info.append("\n\tStart time: " + std::to_string(stream->start_time));
                    av_stream_info.append("\n\tDuration: " + std::to_string((stream->duration)));
            Utils::FormatedPrint::PrintFormated("VideoDecoder::DecodeVideoData", av_stream_info);

            AVCodec* p_local_codec = NULL;
            p_local_codec = avcodec_find_decoder(p_local_codec_parameters->codec_id);

            if (p_local_codec == NULL)
                throw std::runtime_error("Unsupported codec.");
            
            if (p_local_codec_parameters->codec_type == AVMEDIA_TYPE_VIDEO)
            {
                if (video_stream_index == -1)
                {
                    video_stream_index = i;
                    p_codec = p_local_codec;
                    p_codec_parameters = p_local_codec_parameters;
                }

                Utils::FormatedPrint::PrintFormated(
                    "VideoDecoder::DecodeVideoData", 
                    "Video Codec: Resolution [" + std::to_string(p_local_codec_parameters->width) + " x " + std::to_string(p_local_codec_parameters->height) + "]."
                );
            }
            else if (p_local_codec_parameters->codec_type == AVMEDIA_TYPE_AUDIO)
                Utils::FormatedPrint::PrintFormated(
                    "VideoDecoder::DecodeVideoData", 
                    "Audio Codec: " + std::to_string(p_local_codec_parameters->channels) + " channels, sample rate " + std::to_string(p_local_codec_parameters->sample_rate) + "."
                );

            Utils::FormatedPrint::PrintFormated(
                "VideoDecoder::DecodeVideoData",
                "Codec " + string(p_local_codec->name) + " [" + std::to_string(p_local_codec->id) + "] & bit rate " + std::to_string(p_local_codec_parameters->bit_rate)
            );
            
            printf("\n");
        }

        AVCodecContext* p_codec_ctx = avcodec_alloc_context3(p_codec);

        if (!p_codec_ctx)
            throw std::runtime_error("\nFailed to allocate memory for AVCodecContext");
        
        if (avcodec_parameters_to_context(p_codec_ctx, p_codec_parameters) < 0)
            throw std::runtime_error("\nFailed to copy codec params to codec context.");

        if (avcodec_open2(p_codec_ctx, p_codec, NULL) < 0)
            throw std::runtime_error("\nFailed to open codec through avcodec_open2.");

        AVFrame* p_frame = av_frame_alloc();

        if (!p_frame)
            throw std::runtime_error("\nFailed to allocate memory for AVFrame.");
        
        AVPacket* p_packet = av_packet_alloc();

        if (!p_packet)
            throw std::runtime_error("\nFailed to allocate memory for AVPacket.");
        
        int response = 0;
        int packet_to_process = 8;

        while (av_read_frame(av_format_ctx, p_packet) >= 0)
        {
            if (p_packet->stream_index == video_stream_index)
            {
                Utils::FormatedPrint::PrintFormated(
                    "VideoDecoder::DecodeVideoData",
                    " [AVPacket] pts: " + std::to_string(p_packet->pts)
                );

                if (response < 0)
                    break;

                if (--packet_to_process <= 0)
                    break;
            }
            av_packet_unref(p_packet);
        }

        Utils::FormatedPrint::PrintFormated(
            "VideoDecoder::DecodeVideoData",
            "Releasing all resources."
        );

        avformat_close_input(&av_format_ctx);
        av_packet_free(&p_packet);
        av_frame_free(&p_frame);
        avcodec_free_context(&p_codec_ctx);

    }
}