
#include "utils/shader.hpp"

#include "renderer/api.hpp"
#include "renderer/command.hpp"
#include "renderer/camera/orthographic.hpp"

namespace vektor::renderer
{
    RendererApi::API RendererApi::s_API = RendererApi::API::OPENGL;

    std::shared_ptr<camera::Orthographic> Renderer::s_SceneCamera;

    Renderer::SceneInfo *Renderer::cs_SceneData = new Renderer::SceneInfo();

    void Renderer::beginScene(const std::shared_ptr<camera::Orthographic> &camera)
    {
        s_SceneCamera = camera;
        cs_SceneData->ViewProjectionMatrix = s_SceneCamera->getViewProjectionMatrix();
    }

    void Renderer::endScene()
    {
        s_SceneCamera.reset();
    }

    void Renderer::submit(const std::shared_ptr<utils::Shader> &shader, const std::shared_ptr<utils::VertexArray> &vertexArray)
    {
        shader->bindProgram();
        
        shader->setUniformShaderMatrix("u_ViewProjection", cs_SceneData->ViewProjectionMatrix);

        vertexArray->bind();
        renderer::Command::drawIndexed(vertexArray);
    }
}