#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-24
 * 
 * 
 **/

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Layer2D.hpp"
#include "color.hpp"
#include "Font.hpp"
#include "SoundEngine.hpp"

#include <maths/maths.hpp>

#include "renderables/Sprite.hpp"
#include "renderables/Rectangle.hpp"
#include "renderables/Label.hpp"

#include <linmath.h>
#include <iostream>

#include <utils/FileManager.hpp>

#define DEFAULT_WINDOW_WIDTH  300
#define DEFAULT_WINDOW_HEIGHT 300
#define DEFAULT_WINDOW_POSITION_X 0
#define DEFAULT_WINDOW_POSITION_Y 0
#define DEFAULT_WINDOW_NAME "Template application"

namespace Graphics
{
    // class Window;
    class Application
    {
        private:
            static Application* s_Instance;

        private:
            Window* m_Window;

            SoundEngine* m_SoundEngine;


            bool m_Running, m_Suspended;
            uint32_t m_UPS, m_FPS;
            float m_Frametime;

            std::string m_Name;

            std::vector<Layer2D*> m_LayerStack;

        public:
            Application();
            Application(const std::string& name);
            Application(const std::string& name, float width, float height);
            Application(const std::string& name, float x, float y, float width, float height);
            Application(const std::string& name, const Maths::Rectangle& bounds);

            virtual ~Application();

            void AddLayer2D(Layer2D* layer);
            Layer2D* PopLayer2D();
            Layer2D* PopLayer2D(Layer2D* layer);

            void SetWindowHandler(void(* handler)()) { m_Window->SetHandler(handler); };

            void OpenWindow(){ m_Window->Init(); }

            void Start();
            void Suspend();
            void Resume();
            void Stop();

            inline uint32_t GetFPS() const { return m_FPS; }
            inline uint32_t GetUPS() const { return m_UPS; }
            inline float GetFrametime() const { return m_Frametime; }

            inline Window* GetWindow() const { return m_Window; }
            
            void OnRender();

            inline SoundEngine& GetSoundEngine() const{ return *m_SoundEngine; }
        private:

            void Run(){}

            void OnTick();
            void OnUpdate();


        public:
            inline static Application& GetApplication() { return *s_Instance; }
            inline static SoundEngine& GetSoundEngineStatic() { return GetApplication().GetSoundEngine(); }
    };
};