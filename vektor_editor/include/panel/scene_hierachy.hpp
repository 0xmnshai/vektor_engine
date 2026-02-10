#pragma once

#include <imgui.h>

#include "vektor.hpp"

class SceneHierarchyPanel
{
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const std::shared_ptr<vektor::world::scene::Scene> &context);

    void setContext(const std::shared_ptr<vektor::world::scene::Scene> &context);
    void onRender();

    vektor::world::ecs::entity_manager::Entity getSelectedEntity() const { return m_SelectionContext; }
    void setSelectedEntity(vektor::world::ecs::entity_manager::Entity entity);

private:
    void drawEntityNode(vektor::world::ecs::entity_manager::Entity entity);
    void drawContextMenu();

    void reparentEntity(vektor::world::ecs::entity_manager::Entity entity, vektor::world::ecs::entity_manager::Entity newParent);

private:
    std::shared_ptr<vektor::world::scene::Scene> m_Context;
    vektor::world::ecs::entity_manager::Entity m_SelectionContext;

    char m_SearchBuffer[256] = {0};
};
