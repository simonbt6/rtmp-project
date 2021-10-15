#pragma once

#include <iostream>
#include <vector>
#include <inttypes.h>

#include <glad/gl.h>

namespace Graphics
{
    struct VertexBufferElement
    {
        uint32_t type;
        uint32_t count;
        uint32_t normalized;

        static uint32_t GetTypeSize(uint32_t type)
        {
            switch (type)
            {
                case GL_FLOAT:          return 4;
                case GL_UNSIGNED_INT:   return 4;
                case GL_UNSIGNED_BYTE:  return 1;
                default:                return 0;
            };
        }
    };

    class VertexBufferLayout
    {
        private:
            std::vector<VertexBufferElement> m_Elements;
            uint32_t m_Stride;

        public:
        VertexBufferLayout(): m_Stride(0){};
       ~VertexBufferLayout(){};

        template <typename T>
        void Push(uint32_t count)
        {
            static_assert(false);
        }

        template<>
        void Push<float>(uint32_t count)
        {
            m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
            m_Stride += sizeof(float) * count;
        }

        template<>
        void Push<uint32_t>(uint32_t count)
        {
            m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
            m_Stride += sizeof(uint32_t) * count;
        }

        template<>
        void Push<uint8_t>(uint32_t count)
        {
            m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_TRUE });
            m_Stride += sizeof(uint8_t) * count;
        }

        inline uint32_t GetStride() const { return m_Stride; }

        inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
        

        private:
    };
}