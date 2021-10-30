#pragma once
 #define IRRKLANG_STATIC
#include <irrKlang/irrKlang.h>
#include <string>

namespace Graphics
{
    class SoundEngine
    {
        private:
            std::string m_TrackName;
            std::string m_TrackFilepath;

            irrklang::ISoundEngine* m_SoundEngine;

        public:
            SoundEngine();
            ~SoundEngine();

            void Load(const std::string& name, const std::string& filepath);

            void Play();
            void Play(const std::string& name, const std::string& filepath);
            void Pause();
            void Stop();

        private:

    };
};