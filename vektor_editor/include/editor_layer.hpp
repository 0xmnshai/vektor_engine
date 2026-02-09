#pragma once

#include "vektor.hpp"

class EditorLayer : public vektor::layer::Layer
{
public:
    EditorLayer();

    virtual void onAttach() override;
    virtual void onDetach() override;

    virtual void onUpdate(vektor::core::Timestep timestep) override;
    virtual void onRender() override;

    virtual void onEvent(vektor::event::Event &event) override;

private:
    std::shared_ptr<vektor::renderer::camera::Controller> m_CameraController;

    std::shared_ptr<vektor::renderer::Framebuffer> m_Framebuffer;

    std::shared_ptr<vektor::world::scene::Scene> m_ActiveScene;

    ImVec2 m_ViewportSize;
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    std::vector<vektor::world::ecs::entity_manager::Entity> m_Entities;
    vektor::world::ecs::entity_manager::Entity m_SelectedEntity;

    vektor::world::ecs::entity_manager::Entity m_EntityObj;
};