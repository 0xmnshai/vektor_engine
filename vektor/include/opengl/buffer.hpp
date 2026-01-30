#pragma once

#include <vector>
#include <optional>

#include "utils/buffer.hpp"

namespace vektor::opengl
{
    class VertexBuffer : public utils::buffer::Vertex
    {
    public:
        VertexBuffer(const std::vector<float> &vertices);
        virtual ~VertexBuffer();

        virtual void bind() const override;
        virtual void unBind() const override;

        virtual const utils::buffer::Layout &getLayout() const override
        {
            // return m_Layout.value_or(utils::buffer::Layout({}));
            VEKTOR_CORE_ASSERT(m_Layout.has_value(), "VertexBuffer has no layout!");
            return *m_Layout;
        };

        virtual void setLayout(const utils::buffer::Layout &layout) override
        {
            m_Layout = layout;
        };

    private:
        uint32_t m_RendererID;
        std::optional<utils::buffer::Layout> m_Layout;
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