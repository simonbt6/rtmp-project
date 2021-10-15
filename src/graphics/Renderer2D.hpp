?#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-14 
 * 
 * A simple renderer 2D to draw sprites & basic geometry.
 */

#include <glad/gl.h>

#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"

#include "IRenderable2D.hpp"

#include "utils/vec2.hpp"
#include "utils/vec3.hpp"
#include "utils/vec4.hpp"
#include "utils/color.hpp"
#include "utils/rectangle.hpp"

#include <FileManager.hpp>
#include <FormatedPrint.hpp>

#include <map>
#include <string>

namespace Graphics
{

    class IRenderable2d;
    class Renderer2D
    {
        private:
            std::map<std::string, Shader*> m_Shaders;
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

        private:
            Shader* GetShader(const std::string& name);

            void LoadShaders();

    };
};