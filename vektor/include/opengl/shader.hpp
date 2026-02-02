#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "core/core.hpp"
#include "utils/shader.hpp"

namespace vektor::opengl
{
    class VEKTOR_API OpenGLShader : public utils::Shader
    {
    public:
        OpenGLShader(const std::string &filePath);
        OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc);
        ~OpenGLShader();

        virtual void bindProgram() const override;
        virtual void unbindProgram() const override;

        static GLuint compileShader(utils::ShaderType type, const std::string &source);

        virtual void setUniform1i(const std::string &name, const int value) const;
        virtual void setUniform1iv(const std::string &name, const int count, const int *value) const;

        virtual void setUniform1f(const std::string &name, const float value) const;
        virtual void setUniform2f(const std::string &name, const glm::vec2 value) const;
        virtual void setUniform3f(const std::string &name, const glm::vec3 value) const;
        virtual void setUniform4f(const std::string &name, const glm::vec4 value) const;

        virtual void setUniformMat4(const std::string &name, const glm::vec4 &values) const;
        virtual void setUniformShaderMatrix(const std::string &name, const glm::mat4 &matrix) const;
    private:
        GLuint m_ShaderProgram;
        static GLenum toGLType(utils::ShaderType type);
        static void checkCompileErrors(GLuint shader, utils::ShaderType type);
        static void checkLinkErrors(GLuint program);
    };
}