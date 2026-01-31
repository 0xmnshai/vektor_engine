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
        OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc);
        ~OpenGLShader();

        virtual void bindProgram() const override;
        virtual void unbindProgram() const override;

        static GLuint compileShader(utils::ShaderType type, const std::string &source);

        virtual void setUniformShaderMatrix(const std::string &name, const glm::mat4 &matrix) const;
        virtual void setUniformMat4(const std::string &name, const glm::vec4 &values) const;

    private:
        GLuint m_ShaderProgram;
        static GLenum toGLType(utils::ShaderType type);
        static void checkCompileErrors(GLuint shader, utils::ShaderType type);
        static void checkLinkErrors(GLuint program);
    };
}