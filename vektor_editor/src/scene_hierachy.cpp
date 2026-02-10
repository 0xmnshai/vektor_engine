#include "panel/scene_hierachy.hpp"
#include <imgui_internal.h>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

using namespace vektor::world;
using namespace vektor::world::ecs;

static void DrawVec3Control(const std::string &label, glm::vec3 &values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    // X
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    // Y
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    // Z
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::PopID();
}

template <typename T, typename UIFunction>
static void DrawComponent(const std::string &name, entity_manager::Entity entity, UIFunction uiFunction)
{
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_FramePadding;

    if (entity.hasComponent<T>())
    {
        auto &component = entity.getComponent<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
        float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImGui::Separator();

        bool open = ImGui::TreeNodeEx((void *)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
        ImGui::PopStyleVar();

        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        if (ImGui::Button("+", ImVec2{lineHeight, lineHeight}))
        {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings"))
        {
            if (ImGui::MenuItem("Remove component"))
                removeComponent = true;

            ImGui::EndPopup();
        }

        if (open)
        {
            uiFunction(component);
            ImGui::TreePop();
        }

        if (removeComponent)
            entity.removeComponent<T>();
    }
}

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

    ImGui::InputTextWithHint("##Search", "Search Entities...", m_SearchBuffer, sizeof(m_SearchBuffer));
    ImGui::Separator();

    auto &registry = m_Context->getRegistry();

    registry.view<component_storage::TransformComponent, component_storage::TagComponent>()
        .each([this](auto entity, auto &transform, auto &tag)
              {
            if (transform.parent == 0) // Root nodes only
                drawEntityNode({ entity, m_Context.get() }); });

    if (ImGui::BeginPopupContextWindow(0, 1))
    {
        drawContextMenu();
        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Inspector");
    if (m_SelectionContext)
    {
        drawComponents(m_SelectionContext);
    }
    ImGui::End();
}

void SceneHierarchyPanel::drawComponents(vektor::world::ecs::entity_manager::Entity entity)
{
    if (entity.hasComponent<component_storage::TagComponent>())
    {
        auto &tag = entity.getComponent<component_storage::TagComponent>().tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, tag.c_str(), sizeof(buffer) - 1);

        if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
        {
            tag = std::string(buffer);
        }
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent"))
    {
        if (ImGui::MenuItem("Camera"))
        {
            if (!m_SelectionContext.hasComponent<component_storage::CameraComponent>())
                m_SelectionContext.addComponent<component_storage::CameraComponent>();
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::PopItemWidth();

    // 1. Transform
    DrawComponent<component_storage::TransformComponent>("Transform", entity, [](auto &component)
                                                         {
        glm::vec3 translation, rotation, scale;
        glm::quat qRotation;
        glm::vec3 skew;
        glm::vec4 perspective;
        
        glm::decompose(component.localMatrix, scale, qRotation, translation, skew, perspective);
        
        // Convert to degrees for the UI
        rotation = glm::degrees(glm::eulerAngles(qRotation));

        DrawVec3Control("Translation", translation);
        DrawVec3Control("Rotation", rotation);
        DrawVec3Control("Scale", scale, 1.0f);

        // Update Matrix back from user input
        component.localMatrix = glm::translate(glm::mat4(1.0f), translation) *
                                glm::toMat4(glm::quat(glm::radians(rotation))) *
                                glm::scale(glm::mat4(1.0f), scale);
        
        component.markDirty();

        ImGui::Spacing();
        ImGui::ColorEdit4("Color Tint", glm::value_ptr(component.color)); });

    // 2. Camera
    DrawComponent<component_storage::CameraComponent>("Camera", entity, [](auto &component)
                                                      {
        const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
        const char* currentProjectionTypeString = projectionTypeStrings[(int)component.projectionType];
        
        if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
        {
            for (int i = 0; i < 2; i++)
            {
                bool isSelected = (int)component.projectionType == i;
                if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                {
                    component.projectionType = (component_storage::CameraComponent::CameraProjection)i;
                    component.projectionDirty = true;
                }
            }
            ImGui::EndCombo();
        }

        if (component.projectionType == component_storage::CameraComponent::CameraProjection::Perspective)
        {
            if (ImGui::DragFloat("FOV", &component.fovY)) component.projectionDirty = true;
            if (ImGui::DragFloat("Near", &component.nearPlane)) component.projectionDirty = true;
            if (ImGui::DragFloat("Far", &component.farPlane)) component.projectionDirty = true;
        }
        else
        {
            if (ImGui::DragFloat("Size", &component.orthoSize)) component.projectionDirty = true;
            if (ImGui::DragFloat("Near", &component.orthoNear)) component.projectionDirty = true;
            if (ImGui::DragFloat("Far", &component.orthoFar)) component.projectionDirty = true;
        }

        ImGui::Checkbox("Primary", &component.isPrimary); });

    // 3. Script
    DrawComponent<component_storage::NativeScriptComponent>("Native Script", entity, [](auto &component)
                                                            { ImGui::Text("Script Instance: %s", component.instance ? "Running" : "Pending/None"); });
}

void SceneHierarchyPanel::drawEntityNode(entity_manager::Entity entity)
{
    auto &tag = entity.getComponent<component_storage::TagComponent>().tag;
    auto &transform = entity.getComponent<component_storage::TransformComponent>();

    // FIX ID CONFLICTS: Every entity widget is wrapped in a unique ID stack
    ImGui::PushID((int)(uint32_t)entity.getEntity());

    ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0);
    flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (transform.children.empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    bool opened = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());

    if (ImGui::IsItemClicked())
        m_SelectionContext = entity;

    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Create Empty Child"))
        {
            auto child = m_Context->createEntity("Child Entity");
            reparentEntity(child, entity);
        }

        if (ImGui::MenuItem("Delete Entity"))
        {
            if (m_SelectionContext == entity)
                m_SelectionContext = {};
            m_Context->getRegistry().destroy((entt::entity)entity);
        }
        ImGui::EndPopup();
    }

    if (opened)
    {
        for (uint32_t childID : transform.children)
            drawEntityNode({(entt::entity)childID, m_Context.get()});

        ImGui::TreePop();
    }

    ImGui::PopID();  
}

void SceneHierarchyPanel::drawContextMenu()
{
    if (ImGui::MenuItem("Create Empty Entity"))
        m_Context->createEntity("Empty Entity");
}

void SceneHierarchyPanel::reparentEntity(entity_manager::Entity entity, entity_manager::Entity newParent)
{
    auto &transform = entity.getComponent<component_storage::TransformComponent>();

    if (transform.parent != 0)
    {
        auto &oldParentTransform = m_Context->getRegistry().get<component_storage::TransformComponent>((entt::entity)transform.parent);
        auto &children = oldParentTransform.children;
        children.erase(std::remove(children.begin(), children.end(), (uint32_t)entity.getEntity()), children.end());
    }

    if (newParent)
    {
        transform.parent = (uint32_t)newParent.getEntity();
        newParent.getComponent<component_storage::TransformComponent>().children.push_back((uint32_t)entity.getEntity());
    }
    else
    {
        transform.parent = 0;
    }
    transform.markDirty();
}