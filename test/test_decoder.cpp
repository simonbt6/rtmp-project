
#include <codecs/VideoDecoder.hpp>

#include <graphics/Application.hpp>

#include <utils/FileManager.hpp>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>


#define posix_memalign(p, a, s) (((*(p)) = _aligned_malloc((s), (a))), *(p) ?0 :errno)



Maths::Rectangle bounds(0, 0, 1080, 720);
Codecs::VideoDecoder* decoder = new Codecs::VideoDecoder();
uint8_t* frame_data;

bool first_frame = true;

Graphics::Texture* frameTexture;
Graphics::Layer2D* UILayer;

Graphics::Application application("Application test.", bounds);
Graphics::SoundEngine& soundEngine = application.GetSoundEngine();

Maths::mat4 mM;
Maths::mat4 mV;
Maths::mat4 mP;


void playVideoFn()
{
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

    frameTexture = new Graphics::Texture(frame_data, decoder->width, decoder->height);


    Graphics::Primitives::Sprite frame(frameTexture, Graphics::Color(1.0f, 0.0f, 0.0f, 1.0f), Maths::Rectangle(0, 0, 1.0f, 1.0f));
    UILayer->Submit(&frame);


    Graphics::Primitives::Label label("Hello", "font/arial", 40, Graphics::Color(1.0f, 0.0f, 1.0f, 1.0f), Maths::vec2(0, 0));
    // UILayer->Submit(&label);


    application.OnRender();

}


int main()
{   
    try
    {
        printf("\nStarting application.");
        decoder->ReadVideoFile("720p.mp4");

        
        if (posix_memalign((void**)&frame_data, 128, decoder->width * decoder->height * 4) != 0)
            throw std::runtime_error("\nCouldn't allocate frame buffer.");

        application.SetWindowHandler(playVideoFn);
        application.OpenWindow();

        const Maths::vec2& window_size = application.GetWindow()->GetSize();

        mP = Maths::mat4::Orthographic(0.0f, bounds.GetSize().GetX(), 0.0f, bounds.GetSize().GetY(), -1, 1);

        frameTexture = new Graphics::Texture("lilo.jpg");
        UILayer = new Graphics::Layer2D(mP);
        application.AddLayer2D(UILayer);
        
        soundEngine.Play("breakout", "sounds/breakout.mp3");


        application.Start();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}