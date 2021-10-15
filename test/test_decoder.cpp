
#include <VideoCodecs.hpp>

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <window.hpp>


// using namespace std;

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

    std::vector<unsigned char>* data = nullptr;
    
    try
    {
        Graphics::Window* window;
        window->Initialize();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}