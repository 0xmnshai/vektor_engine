#pragma once

#include <glad/glad.h>

#include "utils/texture.hpp"

namespace vektor::opengl
{
    class OpenGLTexture2D : public utils::Texture
    {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height, void *data = nullptr);
        OpenGLTexture2D(const std::string &path);
        virtual ~OpenGLTexture2D();

        inline uint32_t getWidth() const override { return m_Width; }
        inline uint32_t getHeight() const override { return m_Height; }

        virtual void bind(uint32_t slot = 0) const override;
        virtual void unbind() const override;

        virtual void setData(void *data, uint32_t size);

    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };
}