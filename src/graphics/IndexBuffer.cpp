#include "IndexBuffer.hpp"

namespace Graphics
{
    IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count): m_Count(count)
    {
        glGenBuffers(1, &m_RenderID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_RenderID);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}