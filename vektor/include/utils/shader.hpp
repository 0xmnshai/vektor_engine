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
        Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
        ~Shader();

        static GLuint compileShader(ShaderType type, const std::string &source);

        void bindProgram() const;
        void unbindProgram() const;

        void setUniformShaderMatrix(const std::string &name, const glm::mat4 &matrix) const;
        void setUniformMat4(const std::string &name, const glm::vec4 &values) const;

    private:
        static GLenum toGLType(ShaderType type);
        static void checkCompileErrors(GLuint shader, ShaderType type);
        static void checkLinkErrors(GLuint program);

        GLuint m_ShaderProgram;
    };
}