#include "VertexArray.hpp"

namespace Graphics
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_RenderID);
    }


    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RenderID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_RenderID);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
    {
        this->Bind();
        vbo.Bind();

        uint32_t offset = 0;
        const auto& elements = layout.GetElements();
        for (int i = 0; i < elements.size(); i++)
        {
            const VertexBufferElement& element = elements[i];
            glad_glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
            offset += element.count * VertexBufferElement::GetTypeSize(element.type);
        }

        this->Unbind();
    }
};