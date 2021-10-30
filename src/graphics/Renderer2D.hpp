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
#include "TextRenderer.hpp"

#include <maths/maths.hpp>

#include <utils/FileManager.hpp>
#include <utils/FormatedPrint.hpp>

#include <map>
#include <string>

namespace Graphics
{

    
    class IRenderable2D;

    class Renderer2D
    {
        private:
            std::map<std::string, Shader*> m_Shaders;
            static inline const std::string s_FolderPath = "D:/dev-repo/VideoStreaming/RTMP/src/graphics/shaders/";
            Maths::mat4 m_ProjectionMatrix;

            TextRenderer* m_TextRenderer;

            VertexArray m_VAO;


        public:
            Renderer2D(Maths::mat4 projectionMatrix);
           ~Renderer2D();

            void Clear();

            void Draw(const IndexBuffer& ibo, const Shader& shader);
            void Draw(const IRenderable2D& renderable);
            
            void DrawRect(const Maths::Rectangle& rectangle, const Color& color);
            void DrawRect(float width, float height, const Color& color);

            void DrawLine(const Maths::vec2& v1, const Maths::vec2& v2, float thickness, const Color& color);

            void DrawQuad(Maths::vec4 positions, Color color);
            
            void DrawSprite(const Texture& texture, Maths::Rectangle bounds);
            void DrawSprite(const Texture& texture, Maths::vec2 position, Maths::vec2 size);
            void DrawSprite(const Texture& texture, float x, float y, float width, float height);

            void DrawText(const std::string& text, const std::string& font_name, float size,  const Color& color, Maths::vec2 position);
            
        private:
            Shader* GetShader(const std::string& name);

            void LoadShaders();

            void LoadCharacters();

    };
};