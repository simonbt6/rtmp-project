#include "VideoCodecs.hpp"

namespace Codecs
{
    static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);


    VideoDecoder::VideoDecoder()
    {

    }
    
    void VideoDecoder::DecodeVideoData(std::string filename, std::vector<Frame*>* data)
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

        int number = 0;

        while (int frame_number = av_read_frame(av_format_ctx, p_packet) >= 0)
        {
            if (p_packet->stream_index == video_stream_index)
            {
                Utils::FormatedPrint::PrintFormated(
                    "VideoDecoder::DecodeVideoData",
                    " [AVPacket] pts: " + std::to_string(p_packet->pts)
                );

                this->DecodePacket(p_packet, p_codec_ctx, p_frame, data);
                // this->SaveFrameAsJPEG(p_codec_ctx, p_frame, frame_number);


                if (--packet_to_process <= 0)
                    break;
            }
            av_packet_unref(p_packet);
            number++;
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

    void VideoDecoder::DecodePacket(AVPacket* packet, AVCodecContext* av_codec_ctx, AVFrame* frame, std::vector<Frame*>* data)
    {
        // save_to_file(packet->data, packet->size, av_codec_ctx->frame_number);
        int response = avcodec_send_packet(av_codec_ctx, packet);

        if (response < 0)
            throw std::runtime_error("\nFailed to decode packet.");
        
        while (response >= 0)
        {
            response = avcodec_receive_frame(av_codec_ctx, frame);

            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
                break;
            else if (response < 0)
                throw std::runtime_error("Error while receiving a frame from the decoder.");
            
            if (response >= 0)
            {
                string frame_info = "Frame description: \n";
                    frame_info.append("\tType: " + std::to_string(av_codec_ctx->frame_number) + ".\n");
                    frame_info.append("\tSize: " + std::to_string(av_get_picture_type_char(frame->pict_type)) + " bytes.\n");
                    frame_info.append("\tFormat: " + std::to_string(frame->pkt_size) + ".\n");
                    frame_info.append("\tPts: " + std::to_string(frame->pts) + ".\n");
                    frame_info.append("\tKey frame: " + std::to_string(frame->key_frame) + ".\n");
                    frame_info.append("\tDTS: " + std::to_string(frame->coded_picture_number) + ".\n");
                Utils::FormatedPrint::PrintFormated("VideoDecoder::DecodePacket", frame_info);
            }

            if (frame->format != AV_PIX_FMT_YUV420P)
                Utils::FormatedPrint::PrintInfo("Warning: The generated file may not be a grayscale image, but could e.g. be just the R component if the video format is RGB.");
            
            /**
             * Push back to packet buffer 
             * 
             * 
             */

            data->push_back(GetFrameData(av_codec_ctx, frame));
            // save_gray_frame(frame->data[0], frame->linesize[0], frame->width, frame->height, frame_filename);
        }
    }

    VideoDecoder::Frame* VideoDecoder::GetFrameData(AVCodecContext* p_CodecContext, AVFrame* p_Frame)
    {
        Frame* frame = new Frame();

        SwsContext* sws_scaler_context = sws_getContext(p_Frame->width, p_Frame->height, p_CodecContext->pix_fmt,
                                                        p_Frame->width, p_Frame->height, AV_PIX_FMT_RGB0,
                                                        SWS_BILINEAR, NULL, NULL, NULL);
        if (!sws_scaler_context) throw std::runtime_error("\nFailed to initialize sw scaler.");

        uint8_t* data    = new uint8_t[p_Frame->width * p_Frame->height * 4]; 
        uint8_t* dest[4] = {data, NULL, NULL, NULL};
        int dest_linesize[4] = {p_Frame->width * 4, 0, 0, 0};
        sws_scale(sws_scaler_context, p_Frame->data, p_Frame->linesize, 0, p_Frame->height, dest, dest_linesize);
        sws_freeContext(sws_scaler_context);

        frame->data = data;
        frame->width = p_Frame->width;
        frame->height = p_Frame->height;
        
        return frame;
        
    }

    void VideoDecoder::SaveFrameAsJPEG(AVCodecContext* p_CodecCtx, AVFrame* p_Frame, int FrameNo)
    {
        AVCodec* jpegCodec = avcodec_find_encoder(AV_CODEC_ID_JPEG2000);
        if (!jpegCodec) throw std::runtime_error("\nFailed to load jpeg codec.");

        AVCodecContext* jpegCtx = avcodec_alloc_context3(jpegCodec);
        if (!jpegCtx) throw std::runtime_error("\nFailed to allocate jpeg context.");

        jpegCtx->pix_fmt = p_CodecCtx->pix_fmt;
        jpegCtx->width   = p_Frame->width;
        jpegCtx->height  = p_Frame->height;
        jpegCtx->time_base = p_CodecCtx->time_base;

        if (avcodec_open2(jpegCtx, jpegCodec, NULL) < 0) throw std::runtime_error("\nFailed to open jpeg avcodec.");

        FILE *JPEGFile;
        char filename[256];

        AVPacket packet = {.data = NULL, .size = 0};
        av_init_packet(&packet);
        int gotFrame;

        if (avcodec_encode_video2(jpegCtx, &packet, p_Frame, &gotFrame) < 0) throw std::runtime_error("Failed to encode video.");

        sprintf(filename, "dvr-%06d.jpg", FrameNo);
        JPEGFile = fopen(filename, "wb");

        fwrite(packet.data, 1, packet.size, JPEGFile);
        fclose(JPEGFile);

        av_free_packet(&packet);
        avcodec_close(jpegCtx);

    }

    void VideoDecoder::save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename)
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
}