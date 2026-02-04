#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "opengl/texture.hpp"
#include "logger/logger.hpp"

#include "renderer/api.hpp"

namespace vektor::opengl
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string &path) : m_Path(path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        VEKTOR_CORE_ASSERT(data, "Failed to load image!");
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, void *data)
        : m_Width(width), m_Height(height)
    {
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void OpenGLTexture2D::setData(void *data, uint32_t size)
    {
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        VEKTOR_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

    void OpenGLTexture2D::unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

namespace vektor::utils
{
    std::shared_ptr<Texture> Texture::create(const std::string &path)
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::RendererApi::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        case renderer::RendererApi::API::OPENGL:
            return std::make_shared<opengl::OpenGLTexture2D>(path);
        }

        VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
        return nullptr;
    }

    std::shared_ptr<opengl::OpenGLTexture2D> Texture2D::create(const std::string &path)
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::RendererApi::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        case renderer::RendererApi::API::OPENGL:
            return std::make_shared<opengl::OpenGLTexture2D>(path);
        }

        VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
        return nullptr;
    }

    std::shared_ptr<opengl::OpenGLTexture2D> Texture2D::create(uint32_t width, uint32_t height)
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::RendererApi::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        case renderer::RendererApi::API::OPENGL:
            return std::make_shared<opengl::OpenGLTexture2D>(width, height);
        }

        VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
        return nullptr;
    }
}