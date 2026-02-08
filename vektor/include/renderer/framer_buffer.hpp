#pragma once

#include "core/core.hpp"

namespace vektor::renderer
{
    struct FramebufferFormat
    {
        enum class TextureFormat
        {
            None = 0,

            // Color
            RGBA8,
            RED_INTEGER,

            // Depth/stencil
            DEPTH24STENCIL8,

            // Defaults
            Color = RGBA8,
            Depth = DEPTH24STENCIL8
        };
    };

    struct FramebufferSpecification
    {
        uint32_t width = 0, height = 0;
        uint32_t samples = 1;

        bool swapChainTarget = false;

        FramebufferFormat::TextureFormat colorAttachmentFormat = FramebufferFormat::TextureFormat::RGBA8;
        FramebufferFormat::TextureFormat depthAttachmentFormat = FramebufferFormat::TextureFormat::DEPTH24STENCIL8;

        uint32_t stencilAttachmentCount = 0;
        uint32_t colorAttachmentCount = 1;
    };

    class VEKTOR_API Framebuffer
    {
    public:
        virtual void bind() = 0;
        virtual void unBind() = 0;

        virtual const FramebufferSpecification getSpecification() const = 0;

        virtual uint32_t getColorAttachmentRendererID() const = 0;

        virtual void resize(uint32_t width, uint32_t height) = 0;
        virtual void clearAttachment(uint32_t attachmentIndex, int32_t value) = 0;
        virtual void clearDepthStencilAttachment(uint32_t attachmentIndex, float depth, uint8_t stencil) = 0;

        static std::shared_ptr<Framebuffer> create(const FramebufferSpecification &spec);
    };
}