
#include <glad/glad.h>

#include "opengl/vertex_array.hpp"

namespace vektor::opengl
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_RendererID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void VertexArray::unBind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::addVertexBuffer(const std::shared_ptr<utils::buffer::Vertex> &vertexBuffer)
    {
        VEKTOR_CORE_ASSERT(vertexBuffer->getLayout().getElementCount(), "Vertex buffer has no layout!");

        glBindVertexArray(m_RendererID);
        vertexBuffer->bind();

        uint32_t index = 0;
        for (const auto &element : vertexBuffer->getLayout())
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.getComponentCount(),
                element.getType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                vertexBuffer->getLayout().getStride(),
                (const void *)(uintptr_t)element.offset);
            index++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }
    void VertexArray::setIndexBuffer(const std::shared_ptr<utils::buffer::Index> &indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->bind();
        m_IndexBuffer = indexBuffer;
    }
}