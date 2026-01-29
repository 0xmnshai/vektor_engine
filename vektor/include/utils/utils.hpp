#pragma once

#include <string>
#include <glad/glad.h>

#include "core/core.hpp"

namespace vektor::utils
{
    enum class ShaderType
    {
        Vertex,
        Fragment
    };

    class VEKTOR_API ShaderUtils
    {
    public:
        static GLuint compileShader(ShaderType type, const std::string& source);
        static GLuint createProgram(const std::string& vertexSrc, const std::string& fragmentSrc);

    private:
        static GLenum toGLType(ShaderType type);
        static void checkCompileErrors(GLuint shader, ShaderType type);
        static void checkLinkErrors(GLuint program);
    };
}
