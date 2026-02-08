#pragma once

#include "vektor.hpp"

class Sandbox : public vektor::layer::Layer
{
public:
    Sandbox();
    ~Sandbox();

    virtual void onAttach() override;
    virtual void onDetach() override;
    virtual void onUpdate(vektor::core::Timestep timestep) override;
    virtual void onRender() override;
    virtual void onEvent(vektor::event::Event &event) override;

private:
    std::shared_ptr<vektor::renderer::camera::Controller> m_CameraController;

    std::shared_ptr<vektor::utils::ShaderLibrary> m_ShaderLibrary;
    std::shared_ptr<vektor::utils::Texture> m_Texture;
    std::shared_ptr<vektor::utils::buffer::Index> m_IndexBuffer;
    std::shared_ptr<vektor::utils::buffer::Vertex> m_VertexBuffer;
};