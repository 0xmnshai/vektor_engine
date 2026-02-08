#pragma once
#include <string>

#include "core/core.hpp"

namespace vektor::opengl
{
    class OpenGLTexture2D;
}

namespace vektor::utils
{
    class VEKTOR_API Texture
    {
    public:
        virtual ~Texture() = default;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

        virtual void bind(uint32_t slot = 0) const = 0;
        virtual void unbind() const = 0;

        static std::shared_ptr<Texture> create(const std::string &path);

        virtual bool operator==(const Texture &other) const = 0;

        virtual uint32_t getRenderID() const = 0;
    };

    class VEKTOR_API Texture2D : public Texture
    {
    public:
        static std::shared_ptr<opengl::OpenGLTexture2D> create(const std::string &path);
        static std::shared_ptr<opengl::OpenGLTexture2D> create(uint32_t width, uint32_t height);

        virtual ~Texture2D() = default;
    };
}