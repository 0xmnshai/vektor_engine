#pragma once

#include "renderer/framer_buffer.hpp"

namespace vektor::opengl
{
    class VEKTOR_API Framebuffer : public renderer::Framebuffer
    {
    public:
        Framebuffer(const renderer::FramebufferSpecification &specification);
        ~Framebuffer();

        void invalidate();

        virtual void bind() override;
        virtual void unBind() override;

        inline virtual const renderer::FramebufferSpecification getSpecification() const override { return m_FramebufferSpecification; }

        inline virtual uint32_t getColorAttachmentRendererID() const override { return m_ColorAttachement; }

    private:
        renderer::FramebufferSpecification m_FramebufferSpecification;

        uint32_t m_RendererID;
        uint32_t m_ColorAttachement;
        uint32_t m_DepthAttachement;
    };
}