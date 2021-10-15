#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-14 
 * 
 * 
 */
#include "Texture.hpp"
#include "Renderer2D.hpp"

#include "utils/color.hpp"
#include "utils/vec2.hpp"
#include "utils/vec3.hpp"
#include "utils/vec4.hpp"
#include "utils/rectangle.hpp"


#include <inttypes.h>
#include <vector>


namespace Graphics
{
    class IRenderable2D
    {
        protected:

            Maths::Color m_Color;
            Texture* m_Texture;

            Maths::Rectangle m_Bounds;
            std::vector<Maths::vec4<float>> m_UV;
            std::vector<uint32_t> m_Indices;

            bool m_Visible;

        public:
            IRenderable2D(const Maths::vec2<float>& position, const Maths::vec2<float>& size, const Maths::Color& color)
            : m_Bounds(Maths::Rectangle(position, size)), m_Color(color) {}

            virtual ~IRenderable2D(){}

            inline const Maths::vec2<float>& GetPosition() const{ return m_Bounds.GetPosition(); }
            inline void SetPosition(Maths::vec2<float> position) { m_Bounds.SetPosition(position); }

            inline bool IsVisible() const{ return m_Visible; }
            inline void SetVisible(bool value) { m_Visible = value; }

            inline const Maths::vec2<float>& GetSize() const{ return m_Bounds.GetSize(); }
            inline void SetSize(const Maths::vec2<float>& size) { m_Bounds.SetSize(size); }

            inline const Maths::Color& GetColor() const{return m_Color; }
            inline void SetColor(const Maths::Color& color) {m_Color = color; }

            inline const Maths::Rectangle& GetBounds() const{ return m_Bounds; }
            inline void SetBounds(const Maths::Rectangle& bounds) { m_Bounds = bounds; }

            inline const Texture& GetTexture() const 
            { return (m_Texture != nullptr)? *m_Texture : throw std::runtime_error("\nRenderable2D does not contain a texture."); }
            inline void SetTexture(Texture* texture) { m_Texture = texture; }

            inline uint32_t GetVertexCount() const{ return m_UV.size(); }
            inline uint32_t GetIndexCount()  const{ return m_Indices.size(); } 

            inline const Maths::vec4<float>* GetVertices() const{ return &m_UV[0]; }
            inline const uint32_t* GetIndices() const{ return &m_Indices[0]; }

            inline float* GetVerticesArray() const
            {
                float* arr = new float[GetVertexCount() * 4];
                
                for (int i = 0; i < (GetVertexCount() * 4); i += 4)
                {
                    arr[  i  ] = m_UV.at(i / 4).GetX();
                    arr[i + 1] = m_UV.at(i / 4).GetY();
                    arr[i + 2] = m_UV.at(i / 4).GetZ();
                    arr[i + 3] = m_UV.at(i / 4).GetW();
                }
                return arr;
            }

    };

};