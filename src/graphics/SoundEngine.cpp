#include "SoundEngine.hpp"

namespace Graphics
{
    SoundEngine::SoundEngine()
    {
        m_SoundEngine = irrklang::createIrrKlangDevice();
    }

    SoundEngine::~SoundEngine()
    {

    }

    void SoundEngine::Play()
    {

    }

    void SoundEngine::Play(const std::string& name, const std::string& filepath)
    {
        m_SoundEngine->play2D(filepath.c_str(), true);
    }
};