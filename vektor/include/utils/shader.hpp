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

        virtual GLuint compileShader(utils::ShaderType type, const std::string &source) = 0;

        virtual void bindProgram() const = 0;
        virtual void unbindProgram() const = 0;

        virtual const std::string &getName() const = 0;

        static Shader *create(const std::string &filePath);
        static Shader *create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);

    protected:
        GLuint m_ShaderProgram;
    };

    class VEKTOR_API ShaderLibrary
    {
    public:
        static void add(const std::string &name, const std::shared_ptr<Shader> &shader);
        static void add(const std::shared_ptr<Shader> &shader);

        static bool exists(const std::string &name);
        static void remove(const std::string &name);
        static void clear();

        static std::shared_ptr<Shader> load(const std::string &filepath);
        static std::shared_ptr<Shader> load(const std::string &name, const std::string &filepath);
        static std::shared_ptr<Shader> get(const std::string &name);

    private:
        static std::unordered_map<std::string, std::shared_ptr<Shader>> s_Shaders;
    };
}