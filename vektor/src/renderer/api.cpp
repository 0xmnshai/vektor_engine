
#include "opengl/shader.hpp"

#include "renderer/api.hpp"
#include "renderer/command.hpp"
#include "renderer/camera/orthographic.hpp"

namespace vektor::renderer
{
    RendererApi::API RendererApi::s_API = RendererApi::API::OPENGL;

    std::shared_ptr<camera::Orthographic> Renderer::s_SceneCamera;

    Renderer::SceneInfo *Renderer::cs_SceneData = new Renderer::SceneInfo();

    void Renderer::onWindowResize(uint32_t width, uint32_t height)
    {
        glViewport(0, 0, width, height);
    }

    void Renderer::beginScene(const std::shared_ptr<camera::Orthographic> &camera)
    {
        s_SceneCamera = camera;
        cs_SceneData->ViewProjectionMatrix = s_SceneCamera->getViewProjectionMatrix();
    }

    void Renderer::endScene()
    {
        s_SceneCamera.reset();
    }

    void Renderer::init()
    {
        renderer::Command::init();
    }

    void Renderer::submit(
        const std::shared_ptr<utils::Shader> &shader,
        const std::shared_ptr<utils::VertexArray> &vertexArray,
        const glm::mat4 &transform)
    // Taking Material
    {
        std::dynamic_pointer_cast<opengl::OpenGLShader>(shader)->bindProgram();

        std::dynamic_pointer_cast<opengl::OpenGLShader>(shader)->setUniformShaderMatrix("u_ViewProjection", cs_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<opengl::OpenGLShader>(shader)->setUniformShaderMatrix("u_Transform", transform);

        // binding material with shader
        vertexArray->bind();
        renderer::Command::drawIndexed(vertexArray);
    }
}