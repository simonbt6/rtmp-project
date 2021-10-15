#pragma once

/**
 * @author Simon Brisebois-Therrien 
 * @date 2021-10-13
 * 
 */
#include <glad/gl.h>

#include <inttypes.h>

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"



namespace Graphics
{
    class VertexArray
    {
        private:
            uint32_t m_RenderID;

        public:
            VertexArray();
           ~VertexArray();

           void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

            void Bind()   const;
            void Unbind() const;

        private:
    };
}