#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-14 
 * 
 * A simple renderer 2D to draw sprites & basic geometry.
 */

#include <glad/gl.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "color.hpp"

#include "IRenderable2D.hpp"

#include <maths/vec2.hpp>
#include <maths/vec3.hpp>
#include <maths/vec4.hpp>
#include <maths/rectangle.hpp>
#include <maths/mat4.hpp>

#include <utils/FileManager.hpp>
// #include <FormatedPrint.hpp>

#include <map>
#include <string>

namespace Graphics
{
    class IRenderable2d;

    struct TextCharacter
    {
        uint32_t TextureID;

        Maths::vec2<float> Size;
        Maths::vec2<float> Bearing;

        uint32_t Advance;
    };

    class Renderer2D
    {
        private:
            std::map<std::string, Shader*> m_Shaders;
            std::map<char, TextCharacter> m_Characters;
            static inline const std::string s_FolderPath = "D:/dev-repo/VideoStreaming/RTMP/src/graphics/shaders/";

            VertexArray m_VAO;


        public:
            Renderer2D();
           ~Renderer2D();

            void Clear();

            void Draw(const IndexBuffer& ibo, const Shader& shader);
            void Draw(const IRenderable2D& renderable);
            
            void DrawRect(const Maths::Rectangle& rectangle, const Color& color);
            void DrawRect(float width, float height, Maths::vec4<float> color);

            void DrawLine(const Maths::vec2<float>& v1, const Maths::vec2<float>& v2, float thickness, const Color& color);

            void DrawQuad(Maths::vec4<Maths::vec2<float>> positions, Maths::vec4<float> color);

            void DrawSprite(const Texture& texture, float width, float height);

            void DrawText(Maths::vec2<float> position, float scale, const std::string& text, const Color& color);
        private:
            Shader* GetShader(const std::string& name);

            void LoadShaders();

            void LoadCharacters();

    };
};