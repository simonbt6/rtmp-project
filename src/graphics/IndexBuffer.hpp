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
    class IndexBuffer
    {
        private:

            uint32_t m_RenderID;
            uint32_t m_Count;

        public:
            IndexBuffer(const uint32_t* data, uint32_t count);
            ~IndexBuffer();

            void Bind() const;
            void Unbind() const;

            inline uint32_t GetCount() const { return m_Count; }

        private:
    };
}