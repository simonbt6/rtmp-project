#pragma once


#include <ft2build.h>
#include FT_FREETYPE_H

#include "Texture.hpp"

#include <maths/vec2.hpp>

#include <string>

namespace Graphics
{
    class Font
    {
        private:
            std::string m_Filename;
            std::string m_Name;


            Maths::vec2<float> m_Scale;

        public:
            Font(const std::string& name, const std::string& filename, float size);
            Font(const std::string& name, const uint8_t* data, uint32_t datasize, float size);

            void SetScale(float x, float y);

            inline void SetScale(const Maths::vec2<float>& scale) { m_Scale = scale; }
            inline const Maths::vec2<float>& GetScale() const { return m_Scale; }

            
    };
};