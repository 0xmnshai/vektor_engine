#pragma once

#include "core/core.hpp"
#include "utils/buffer.hpp"

namespace vektor::utils
{
    class VEKTOR_API VertexArray
    {
    public:
        virtual ~VertexArray() = default;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        virtual void addVertexBuffer(const std::shared_ptr<buffer::Vertex> &vertexBuffer) = 0;
        virtual void setIndexBuffer(const std::shared_ptr<buffer::Index> &indexBuffer) = 0;

        // virtual void setLayout(const buffer::Layout &layout) = 0

        virtual const std::vector<std::shared_ptr<buffer::Vertex>> &getVertexBuffers() const = 0;
        virtual const std::shared_ptr<buffer::Index> &getIndexBuffer() const = 0;

        static VertexArray *create();
    };
}