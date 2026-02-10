#include "panel/scene_hierachy.hpp"
#include <imgui_internal.h>
#include <algorithm>

using namespace vektor::world;
using namespace vektor::world::ecs;

SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<scene::Scene> &context)
{
    setContext(context);
}

void SceneHierarchyPanel::setContext(const std::shared_ptr<scene::Scene> &context)
{
    m_Context = context;
    m_SelectionContext = {};
}

void SceneHierarchyPanel::onRender()
{
    ImGui::Begin("Scene Hierarchy");

    if (!m_Context)
    {
        ImGui::End();
        return;
    }

    ImGui::InputTextWithHint("Search", "Search Entities...", m_SearchBuffer, sizeof(m_SearchBuffer));
    // ImGui::InputTextWithHint("##Search", "Search Entities...", m_SearchBuffer, sizeof(m_SearchBuffer));
    ImGui::Separator();

    auto &registry = m_Context->getRegistry();
    auto view = registry.view<
        component_storage::TransformComponent,
        component_storage::TagComponent>();

    view.each([this](auto entity, auto &transform, auto &tag)
              {
            if (transform.parent == 0) // root only
                drawEntityNode({entity, m_Context.get()}); });

    if (ImGui::BeginPopupContextWindow(nullptr, 1))
    {
        drawContextMenu();
        ImGui::EndPopup();
    }

    ImGui::End();
}

void SceneHierarchyPanel::drawEntityNode(entity_manager::Entity entity)
{
    auto &tag = entity.getComponent<component_storage::TagComponent>().getTag();
    auto &transform = entity.getComponent<component_storage::TransformComponent>();

    // Search filter
    if (strlen(m_SearchBuffer) > 0)
    {
        std::string tagLower = tag;
        std::string search = m_SearchBuffer;
        std::transform(tagLower.begin(), tagLower.end(), tagLower.begin(), ::tolower);
        std::transform(search.begin(), search.end(), search.begin(), ::tolower);

        if (tagLower.find(search) == std::string::npos)
            return;
    }

    ImGuiTreeNodeFlags flags =
        ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (transform.children.empty())
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    flags |= ImGuiTreeNodeFlags_OpenOnArrow;

    bool opened = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags,
                                    "%s-%u", tag.c_str(), (uint32_t)entity);

    if (ImGui::IsItemClicked())
        m_SelectionContext = entity;

    if (ImGui::BeginPopupContextItem("EntityContext")) //    if (ImGui::BeginPopupContextItem())
    {
        m_SelectionContext = entity;

        if (ImGui::MenuItem("Create Child"))
        {
            auto child = m_Context->createEntity("Child");
            reparentEntity(child, entity);
        }

        if (ImGui::MenuItem("Delete"))
        {
            m_Context->getRegistry().destroy((entt::entity)entity);
            ImGui::EndPopup();
            return;
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginDragDropSource())
    {
        uint32_t id = (uint32_t)entity;
        ImGui::SetDragDropPayload("VEKTOR_ENTITY", &id, sizeof(uint32_t));
        ImGui::Text("%s", tag.c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("VEKTOR_ENTITY"))
        {
            uint32_t dropped = *(uint32_t *)payload->Data;
            reparentEntity({(entt::entity)dropped, m_Context.get()}, entity);
        }
        ImGui::EndDragDropTarget();
    }

    if (opened && !transform.children.empty())
    {
        for (uint32_t child : transform.children)
            drawEntityNode({(entt::entity)child, m_Context.get()});

        ImGui::TreePop();
    }
}

void SceneHierarchyPanel::drawContextMenu()
{
    if (ImGui::MenuItem("Create Empty"))
        m_Context->createEntity("Empty");

    if (ImGui::MenuItem("Create Camera"))
    {
        auto e = m_Context->createEntity("Camera");
        e.addComponent<component_storage::CameraComponent>();
    }
}

void SceneHierarchyPanel::reparentEntity(entity_manager::Entity entity,
                                         entity_manager::Entity newParent)
{
    auto &transform = entity.getComponent<component_storage::TransformComponent>();

    if (transform.parent != 0)
    {
        auto &oldParent =
            m_Context->getRegistry().get<component_storage::TransformComponent>(
                (entt::entity)transform.parent);

        auto &children = oldParent.children;
        children.erase(std::remove(children.begin(), children.end(), (uint32_t)entity),
                       children.end());
    }

    if (newParent)
    {
        transform.parent = (uint32_t)newParent;
        auto &newParentTransform =
            newParent.getComponent<component_storage::TransformComponent>();
        newParentTransform.children.push_back((uint32_t)entity);
    }
    else
    {
        transform.parent = 0;
    }

    transform.markDirty();
}