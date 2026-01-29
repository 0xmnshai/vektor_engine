
#include <glad/glad.h>

#include "core/core.hpp"
#include "utils/utils.hpp"
#include "logger/logger.hpp"

namespace vektor::utils
{
    GLenum ShaderUtils::toGLType(ShaderType type)
    {
        switch (type)
        {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        }
        return 0;
    }

    GLuint ShaderUtils::compileShader(ShaderType type, const std::string &source)
    {
        GLenum glType = toGLType(type);
        GLuint shader = glCreateShader(glType);

        const char *src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        checkCompileErrors(shader, type);

        return shader;
    }

    GLuint ShaderUtils::createProgram(const std::string &vertexSrc,
                                      const std::string &fragmentSrc)
    {
        GLuint program = glCreateProgram();

        GLuint vs = compileShader(ShaderType::Vertex, vertexSrc);
        GLuint fs = compileShader(ShaderType::Fragment, fragmentSrc);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        checkLinkErrors(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    void ShaderUtils::checkCompileErrors(GLuint shader, ShaderType type)
    {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            GLint length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            std::string log(length, '\0');
            glGetShaderInfoLog(shader, length, nullptr, log.data());

            const char *typeStr =
                (type == ShaderType::Vertex) ? "VERTEX" : "FRAGMENT";

            VEKTOR_CORE_ERROR("{} SHADER COMPILATION FAILED:\n{}", typeStr, log);
            glDeleteShader(shader);
        }
    }

    void ShaderUtils::checkLinkErrors(GLuint program)
    {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success)
        {
            GLint length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

            std::string log(length, '\0');
            glGetProgramInfoLog(program, length, nullptr, log.data());

            VEKTOR_CORE_ERROR("SHADER PROGRAM LINK FAILED:\n{}", log);
            glDeleteProgram(program);
        }
    }
}
