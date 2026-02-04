#pragma once

#include "vektor.hpp"

class Sandbox2D : public vektor::layer::Layer
{
public:
    Sandbox2D();

    virtual void onAttach() override;
    virtual void onDetach() override;

    virtual void onUpdate(vektor::core::Timestep timestep) override;
    virtual void onRender() override;

    virtual void onEvent(vektor::event::Event &event) override;

private:
    std::shared_ptr<vektor::renderer::camera::Controller> m_CameraController;

    vektor::utils::ShaderLibrary m_ShaderLibrary;
    std::shared_ptr<vektor::utils::VertexArray> m_VertexArray;

    std::shared_ptr<vektor::utils::buffer::Index> m_IndexBuffer;
    std::shared_ptr<vektor::utils::buffer::Vertex> m_VertexBuffer;

    glm::vec3 m_Position;

    glm::vec4 m_Color = {0.2f, 0.3f, 0.8f, 1.0f};
};