#pragma once

#include "utils/buffer.hpp"
#include <vector>

namespace vektor::opengl
{
    class VertexBuffer : public utils::buffer::Vertex
    {
    public:
        VertexBuffer(const std::vector<float> &vertices);
        virtual ~VertexBuffer();

        virtual void bind() const override;
        virtual void unBind() const override;

    private:
        uint32_t m_RendererID;
    };

    class IndexBuffer : public utils::buffer::Index
    {
    public:
        IndexBuffer(const std::vector<uint32_t> &indices);
        virtual ~IndexBuffer();

        virtual void bind() const override;
        virtual void unBind() const override;

        virtual uint32_t getCount() const override { return m_Count; }

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
}