#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/core.hpp"
#include "opengl/shader.hpp"
#include "logger/logger.hpp"

namespace vektor::opengl
{
    opengl::OpenGLShader::OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        m_ShaderProgram = glCreateProgram();

        GLuint vs = compileShader(utils::ShaderType::Vertex, vertexSrc);
        GLuint fs = compileShader(utils::ShaderType::Fragment, fragmentSrc);

        glAttachShader(m_ShaderProgram, vs);
        glAttachShader(m_ShaderProgram, fs);
        glLinkProgram(m_ShaderProgram);

        opengl::OpenGLShader::checkLinkErrors(m_ShaderProgram);

        glDeleteShader(vs);
        glDeleteShader(fs);
    };

    opengl::OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_ShaderProgram);
    }

    GLenum opengl::OpenGLShader::toGLType(utils::ShaderType type)
    {
        switch (type)
        {
        case utils::ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case utils::ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        }
        return 0;
    }

    GLuint opengl::OpenGLShader::compileShader(utils::ShaderType type, const std::string &source)
    {
        GLenum glType = toGLType(type);
        GLuint shader = glCreateShader(glType);

        const char *src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        checkCompileErrors(shader, type);

        return shader;
    }

    void opengl::OpenGLShader::checkCompileErrors(GLuint shader, utils::ShaderType type)
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
                (type == utils::ShaderType::Vertex) ? "VERTEX" : "FRAGMENT";

            VEKTOR_CORE_ERROR("{} SHADER COMPILATION FAILED:\n{}", typeStr, log);
            glDeleteShader(shader);
        }
    }

    void opengl::OpenGLShader::checkLinkErrors(GLuint program)
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

    void opengl::OpenGLShader::bindProgram() const
    {
        glUseProgram(m_ShaderProgram);
    }

    void opengl::OpenGLShader::unbindProgram() const
    {
        // glDeleteProgram(m_ShaderProgram);
        glUseProgram(0);
    }

    // ------------------------------------------------------------------------
    // Uniform Implementations
    // ------------------------------------------------------------------------

    void opengl::OpenGLShader::setUniform1i(const std::string &name, const int value) const
    {
        GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
        glUniform1i(location, value);
    }

    void opengl::OpenGLShader::setUniform1iv(const std::string &name, const int count, const int *value) const
    {
        GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
        glUniform1iv(location, count, value);
    }

    void opengl::OpenGLShader::setUniform1f(const std::string &name, const float value) const
    {
        GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
        glUniform1f(location, value);
    }

    void opengl::OpenGLShader::setUniform2f(const std::string &name, const glm::vec2 value) const
    {
        GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void opengl::OpenGLShader::setUniform3f(const std::string &name, const glm::vec3 value) const
    {
        GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void opengl::OpenGLShader::setUniform4f(const std::string &name, const glm::vec4 value) const
    {
        GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void opengl::OpenGLShader::setUniformMat4(const std::string &name, const glm::vec4 &values) const
    {
        GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void opengl::OpenGLShader::setUniformShaderMatrix(const std::string &name, const glm::mat4 &matrix) const
    {
        GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}