#pragma once

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-10-13 
 * 
 */

#include <glad/gl.h>

#include <inttypes.h>



namespace Graphics
{
    class VertexBuffer
    {
        private:
            uint32_t m_RenderID;

        public:
            VertexBuffer(const void* data, uint32_t size);
            ~VertexBuffer();

            void Bind() const;
            void Unbind() const;

    };
}