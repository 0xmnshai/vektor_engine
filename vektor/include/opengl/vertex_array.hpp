#pragma once

#include <vector>

#include "utils/buffer.hpp"
#include "utils/vertex_array.hpp"

namespace vektor::opengl
{
    class VertexArray : public utils::VertexArray
    {
    public:
        VertexArray();
        virtual ~VertexArray();

        virtual void bind() const override;
        virtual void unBind() const override;

        virtual void addVertexBuffer(const std::shared_ptr<utils::buffer::Vertex> &vertexBuffer) override;
        virtual void setIndexBuffer(const std::shared_ptr<utils::buffer::Index> &indexBuffer) override;

    private:
        std::vector<std::shared_ptr<utils::buffer::Vertex>> m_VertexBuffers;
        std::shared_ptr<utils::buffer::Index> m_IndexBuffer;
        uint32_t m_RendererID;
    };
}