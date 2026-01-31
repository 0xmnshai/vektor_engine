#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "core/core.hpp"

namespace vektor::utils
{
    enum class ShaderType
    {
        Vertex,
        Fragment
    };

    class VEKTOR_API Shader
    {
    public: 
        virtual ~Shader() = default;

        static GLuint compileShader(ShaderType type, const std::string &source);

        virtual void bindProgram() const = 0;
        virtual void unbindProgram() const = 0;

        static Shader *create(const std::string &vertexSrc, const std::string &fragmentSrc);

    protected: 
        GLuint m_ShaderProgram;
    };
}