
#include <VideoCodecs.hpp>
#include <StreamDecoder.hpp>

#include <Window.hpp>

#include <FileManager.hpp>
#include <Memory.hpp>

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <window.hpp>


std::vector<unsigned char>* read(string path)
{
    std::string line;
    std::vector<string> strdata;
    std::vector<unsigned char>* data = new std::vector<unsigned char>();
    std::ifstream file(path);
    
    while(std::getline(file, line))
    {
        std::string buffer;
        std::stringstream ss;
        ss << line;
        
        while (std::getline(ss, buffer, ' '))
        {
            data->push_back(std::stoul(buffer, nullptr, 16));
        }
    }
    file.close();
    std::cout << "Data size: " << data->size() << std::endl;
    return data;
}

int main()
{
    Codecs::VideoDecoder* video_decoder = new Codecs::VideoDecoder();
    Codecs::StreamDecoder* stream_decoder = new Codecs::StreamDecoder(AVCodecID::AV_CODEC_ID_H264);

    std::vector<uint8_t>* data = read("data/videodata.bin");
    
    try
    {
        // AVPacket* packet;
        // video_decoder->DecodeVideoData("data/bunny_video.mp4", data);
        // packet = stream_decoder->ParsePacket(data->data(), data->size());

        Graphics::Window window;
        window.Initialize();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}