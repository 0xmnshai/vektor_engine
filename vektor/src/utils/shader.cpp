
#include "renderer/api.hpp"
#include "utils/shader.hpp"

namespace vektor::utils
{
    utils::Shader *utils::Shader::create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::RendererApi::API::OPENGL:
            return new opengl::OpenGLShader(name, vertexSrc, fragmentSrc);
        case renderer::RendererApi::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        default:
            VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
            return nullptr;
        }
    }

    utils::Shader *utils::Shader::create(const std::string &filepath)
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::RendererApi::API::OPENGL:
            return new opengl::OpenGLShader(filepath);
        case renderer::RendererApi::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        default:
            VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
            return nullptr;
        }
    }

    std::unordered_map<std::string, std::shared_ptr<utils::Shader>> utils::ShaderLibrary::s_Shaders;

    void utils::ShaderLibrary::add(const std::string &name, const std::shared_ptr<utils::Shader> &shader)
    {
        std::string shaderName = shader->getName();
        VEKTOR_CORE_ASSERT(!exists(shaderName), "Shader already exists with that name!");
        s_Shaders[shaderName] = shader;
    }

    void utils::ShaderLibrary::add(const std::shared_ptr<utils::Shader> &shader)
    {
        std::string shaderName = shader->getName();
        VEKTOR_CORE_ASSERT(!exists(shaderName), "Shader already exists with that name!");
        s_Shaders[shaderName] = shader;
    }

    bool utils::ShaderLibrary::exists(const std::string &name)
    {
        return s_Shaders.find(name) != s_Shaders.end();
    }

    void utils::ShaderLibrary::remove(const std::string &name)
    {
        VEKTOR_CORE_ASSERT(exists(name), "Shader does not exist with that name!");
        s_Shaders.erase(name);
    }

    void utils::ShaderLibrary::clear()
    {
        s_Shaders.clear();
    }

    std::shared_ptr<Shader> utils::ShaderLibrary::load(const std::string &filepath)
    {
        std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(Shader::create(filepath));
        add(shader);
        return shader;
    }

    std::shared_ptr<Shader> utils::ShaderLibrary::load(const std::string &name, const std::string &filepath)
    {
        std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(Shader::create(filepath));
        add(name, shader);
        return shader;
    }
    std::shared_ptr<Shader> utils::ShaderLibrary::get(const std::string &name)
    {
        VEKTOR_CORE_ASSERT(exists(name), "Shader does not exist with that name!");
        return s_Shaders[name];
    }
}