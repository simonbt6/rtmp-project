
#include <codecs/VideoDecoder.hpp>

#include <graphics/Window.hpp>

#include <utils/FileManager.hpp>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

#include <window.hpp>
#include <Renderer2D.hpp>

#define posix_memalign(p, a, s) (((*(p)) = _aligned_malloc((s), (a))), *(p) ?0 :errno)

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
int signX = 1, signY = 1;

// constants
Maths::vec2 V0 (0.003f, 0.003f); // Initial velocity

#define DeltaV sqrt(pow(V0.GetX(), 2) + pow(V0.GetY(), 2)) 
#define deg0 asin((V0.GetY()*sin(90)) / DeltaV)
#define VHeight(t) (t * (V0 * sin(deg0)) - 16 * pow(t, 2))
#define TOF (DeltaV * sin(deg0) / 16)
#define kineticEnergy(m) (0.5f * m * pow(DeltaV, 2))

#define MAXH (pow(V0.GetY(), 2)/(2*Gravity))

float Gravity = 0.01f;


// variables

Maths::vec2 direction(0.5f, 0.3f);
Maths::vec2 position(0.0f, 0.0f);

Maths::Rectangle window_bounds(0.0f, 0.0f, 1.0f, 1.0f);
void gamerenderFn(Graphics::Renderer2D& renderer)
{
    float Epotential = 0;
    float masse = 10;
    float Ekinetic = kineticEnergy(masse);
    float TimeOfFlight = TOF;
    printf("\nKinetic energy: %f", Ekinetic);
    
    renderer.DrawRect(Maths::Rectangle(position.GetX(), position.GetY(), 0.01f, 0.01f), Graphics::Color(0.5f, 0.3f, 0.8f, 1.0f));
    if (window_bounds.GetSize().GetX() <= abs(position.GetX())) direction *= Maths::vec2(-1.0f,  1.0f);
    if (window_bounds.GetSize().GetY() <= abs(position.GetY())) direction *= Maths::vec2( 1.0f, -1.0f);
    
    direction -= Maths::vec2(0.0f, Gravity); 
    position += direction * V0;
}




Maths::Rectangle bounds(0, 0, 0.8f, 0.8f);
Codecs::VideoDecoder* decoder = new Codecs::VideoDecoder();
uint8_t* frame_data;

bool first_frame = true;

void playVideoFn(Graphics::Renderer2D& renderer)
{
    renderer.DrawRect(bounds, Graphics::Color(0.0f, 0.5f, 1.0f, 1.0f));

    int64_t pts;
    decoder->ReadFrame(frame_data, &pts);

    if (first_frame) 
    {
        glfwSetTime(0.0);
        first_frame = false;
    }

    double pt_in_seconds = pts * (double)decoder->time_base.num / (double)decoder->time_base.den;
    while (pt_in_seconds > glfwGetTime()) 
    {
            glfwWaitEventsTimeout((unsigned)(pt_in_seconds - glfwGetTime()));
    }

    // renderer.DrawSprite(Graphics::Texture(frame_data, decoder->width, decoder->height), bounds.GetSize().GetX(), bounds.GetSize().GetY());
    renderer.DrawText(Maths::vec2<float>(0.0f, 0.0f), 10.0f, "hello", Graphics::Color(1.0f, 0.0f, 0.0f, 1.0f));
}



int main()
{   
    try
    {
        decoder->ReadVideoFile("test.mp4");
        
        if (posix_memalign((void**)&frame_data, 128, decoder->width * decoder->height * 4) != 0)
            throw std::runtime_error("\nCouldn't allocate frame buffer.");

        Graphics::Window window;
        window.Initialize();

        // window.Loop(gamerenderFn);
        window.Loop(playVideoFn);
        decoder->Close();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}