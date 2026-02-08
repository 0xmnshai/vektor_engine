#include <glad/glad.h>

#include "logger/logger.hpp"
#include "opengl/framer_buffer.hpp"

namespace vektor::opengl
{
    opengl::Framebuffer::Framebuffer(const renderer::FramebufferSpecification &spec)
        : m_FramebufferSpecification(spec), m_RendererID(0), m_ColorAttachement(0), m_DepthAttachement(0)
    {
        invalidate();
    }

    opengl::Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachement);
        glDeleteTextures(1, &m_DepthAttachement);
    }

    void opengl::Framebuffer::invalidate()
    {
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachement);
            glDeleteTextures(1, &m_DepthAttachement);

            m_ColorAttachement = 0;
            m_DepthAttachement = 0;
        }

        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        glGenTextures(1, &m_ColorAttachement);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachement);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_FramebufferSpecification.width, m_FramebufferSpecification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachement, 0);

        glGenTextures(1, &m_DepthAttachement);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachement);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_FramebufferSpecification.width, m_FramebufferSpecification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachement, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            VEKTOR_CORE_ASSERT(false, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void opengl::Framebuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_FramebufferSpecification.width, m_FramebufferSpecification.height);
    }

    void opengl::Framebuffer::unBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}