

#include <imgui_internal.h>

#include "panel/scene_hierachy.hpp"

SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<vektor::world::scene::Scene> &context)
{
    setContext(context);
}

void SceneHierarchyPanel::setContext(const std::shared_ptr<vektor::world::scene::Scene> &context)
{
    m_Context = context;
    m_SelectionContext = {};
}

void SceneHierarchyPanel::onRender()
{
    ImGui::Begin("Scene Hierarchy");

    if (m_Context)
    {
        ImGui::InputTextWithHint("##Search", "Search Entities...", m_SearchBuffer, sizeof(m_SearchBuffer));
        ImGui::Separator();

        auto view = m_Context->getRegistry().view<vektor::world::ecs::component_storage::TagComponent>();
        for (auto entity : view)
        {
            auto &transform = view.get<vektor::world::ecs::component_storage::TagComponent>(entity);
            std::string tag = transform.getTag() + "-" + std::to_string((uint32_t)entity);

            ImGui::Text("%s", tag.c_str());

            drawEntityNode({entity, m_Context.get()});
        }
    }

    ImGui::End();
}

void SceneHierarchyPanel::drawEntityNode(vektor::world::ecs::entity_manager::Entity entity)
{

    auto &tag = entity.getComponent<vektor::world::ecs::component_storage::TagComponent>();
    std::string tagStr = tag.getTag() + "-" + std::to_string((uint32_t)entity);

    ImGuiTreeNodeFlags flags =
        ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    ImGui::PushID((uint32_t)entity);

    bool opened = ImGui::TreeNodeEx("##entity", flags, "%s", tagStr.c_str());

    if (ImGui::IsItemClicked())
        m_SelectionContext = entity;

    if (opened)
        ImGui::TreePop();

    ImGui::PopID();
}

// #include <imgui_internal.h>

// #include "panel/scene_hierachy.hpp"

// SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<vektor::world::scene::Scene> &context)
// {
//     setContext(context);
// }

// void SceneHierarchyPanel::setContext(const std::shared_ptr<vektor::world::scene::Scene> &context)
// {
//     m_Context = context;
//     m_SelectionContext = {};
// }

// void SceneHierarchyPanel::onRender()
// {
//     ImGui::Begin("Scene Hierarchy");

//     if (m_Context)
//     {
//         ImGui::InputTextWithHint("##Search", "Search Entities...", m_SearchBuffer, sizeof(m_SearchBuffer));
//         ImGui::Separator();

//         auto view = m_Context->getRegistry().view<vektor::world::ecs::component_storage::TransformComponent>();
//         for (auto entityID : view)
//         {
//             auto &transform = view.get<vektor::world::ecs::component_storage::TransformComponent>(entityID);

//             if (transform.parent == 0)
//             {
//                 drawEntityNode({entityID, m_Context.get()});
//             }
//         }

//         if (ImGui::BeginPopupContextWindow(0, 1))
//         {
//             drawContextMenu();
//             ImGui::EndPopup();
//         }
//     }

//     ImGui::End();
// }

// void SceneHierarchyPanel::drawEntityNode(vektor::world::ecs::entity_manager::Entity entity)
// {
//     auto &tag = entity.getComponent<vektor::world::ecs::component_storage::TagComponent>().tag;
//     auto &transform = entity.getComponent<vektor::world::ecs::component_storage::TransformComponent>();

//     ImGui::PushID((int)(uint32_t)entity);

//     if (strlen(m_SearchBuffer) > 0)
//     {
//         std::string tagLower = tag;
//         std::transform(tagLower.begin(), tagLower.end(), tagLower.begin(), ::tolower);
//         std::string searchLower = m_SearchBuffer;
//         std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
//         if (tagLower.find(searchLower) == std::string::npos)
//             return;
//     }

//     ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0);
//     flags |= ImGuiTreeNodeFlags_OpenOnArrow;
//     flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

//     if (transform.children.empty())
//         flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

//     // bool opened = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags, tag.c_str());

//     bool opened = ImGui::TreeNodeEx(tag.c_str(), flags);

//     if (ImGui::IsItemClicked())
//     {
//         m_SelectionContext = entity;
//     }

//     if (ImGui::BeginPopupContextItem())
//     {
//         m_SelectionContext = entity;
//         if (ImGui::MenuItem("Create Child Entity"))
//         {
//             auto child = m_Context->createEntity("Child Entity");
//             reparentEntity(child, entity);
//         }
//         ImGui::Separator();
//         if (ImGui::MenuItem("Delete Entity"))
//         {
//             if (m_SelectionContext == entity)
//                 m_SelectionContext = {};
//             m_Context->getRegistry().destroy(entity);
//             ImGui::EndPopup();
//         }
//         ImGui::EndPopup();
//     }

//     if (ImGui::BeginDragDropSource())
//     {
//         uint32_t entityID = (uint32_t)entity;
//         ImGui::SetDragDropPayload("PROJECT_EXPLORER_ENTITY", &entityID, sizeof(uint32_t));
//         ImGui::Text("%s", tag.c_str());
//         ImGui::EndDragDropSource();
//     }

//     if (ImGui::BeginDragDropTarget())
//     {
//         if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("PROJECT_EXPLORER_ENTITY"))
//         {
//             uint32_t droppedEntityID = *(const uint32_t *)payload->Data;
//             reparentEntity({(entt::entity)droppedEntityID, m_Context.get()}, entity);
//         }
//         ImGui::EndDragDropTarget();
//     }

//     if (opened && !transform.children.empty())
//     {
//         for (auto childID : transform.children)
//         {
//             drawEntityNode({(entt::entity)childID, m_Context.get()});
//         }
//         ImGui::TreePop();
//     }

//     ImGui::PopID();
// }

// void SceneHierarchyPanel::drawContextMenu()
// {
//     if (ImGui::MenuItem("Create Empty Entity"))
//         m_Context->createEntity("Empty Entity");

//     if (ImGui::MenuItem("Create Camera"))
//     {
//         auto e = m_Context->createEntity("Camera");
//         e.addComponent<vektor::world::ecs::component_storage::CameraComponent>();
//     }
// }

// void SceneHierarchyPanel::reparentEntity(vektor::world::ecs::entity_manager::Entity entity, vektor::world::ecs::entity_manager::Entity newParent)
// {
//     auto &transform = entity.getComponent<vektor::world::ecs::component_storage::TransformComponent>();

//     if (transform.parent != 0)
//     {
//         auto &oldParentTransform = m_Context->getRegistry().get<vektor::world::ecs::component_storage::TransformComponent>((entt::entity)transform.parent);
//         auto &children = oldParentTransform.children;
//         children.erase(std::remove(children.begin(), children.end(), (uint32_t)entity), children.end());
//     }

//     if (newParent)
//     {
//         transform.parent = (uint32_t)newParent;
//         auto &newParentTransform = newParent.getComponent<vektor::world::ecs::component_storage::TransformComponent>();
//         newParentTransform.children.push_back((uint32_t)entity);
//     }
//     else
//     {
//         transform.parent = 0;
//     }

//     transform.markDirty();
// }
