#include "editor_layer.hpp"

namespace entity_manager = vektor::world::ecs::entity_manager;

glm::vec3 clearColor = glm::vec3(0.1f, 0.1f, 0.1f);

EditorLayer::EditorLayer() : Layer("Editor Layer")
{
  float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
  m_CameraController =
      std::make_shared<vektor::renderer::camera::Controller>(aspectRatio);
  m_ActiveScene = std::make_shared<vektor::world::scene::Scene>();

  m_CameraObj = m_ActiveScene->createEntity("Camera");
  auto &cameraTransform = m_CameraObj.addComponent<
      vektor::world::ecs::component_storage::CameraComponent>();
  cameraTransform.setProjectionMatrix(
      glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f));
}

void EditorLayer::onAttach()
{
  VEKTOR_CORE_INFO("EditorLayer layer attached");

  auto framebufferSpecification = vektor::renderer::FramebufferSpecification();
  framebufferSpecification.width = WINDOW_WIDTH;
  framebufferSpecification.height = WINDOW_HEIGHT;

  m_Framebuffer =
      vektor::renderer::Framebuffer::create(framebufferSpecification);

  for (int i = 0; i < 3; i++)
  {
    m_EntityObj = m_ActiveScene->createEntity("Square");
    auto &transform = m_EntityObj.getComponent<
        vektor::world::ecs::component_storage::TransformComponent>();

    transform.translate({i * 1.2f, 0.0f, 0.0f});
    transform.color = {1, 0, 0, 1};

    m_Entities.push_back(m_EntityObj);
  }
}
void EditorLayer::onDetach() { VEKTOR_CORE_INFO("EditorLayer layer detached"); }

void EditorLayer::onUpdate(vektor::core::Timestep timestep)
{
  VEKTOR_PROFILE_FUNCTION();

  VEKTOR_PROFILE_BEGIN_SESSION("EditorLayer::OnUpdate",
                               "EditorLayer-Profile-Runtime.json");

  auto stats = m_Framebuffer->getSpecification();
  if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
      (stats.width != m_ViewportSize.x || stats.height != m_ViewportSize.y))
  {
    m_Framebuffer->resize((uint32_t)m_ViewportSize.x,
                          (uint32_t)m_ViewportSize.y);
    m_CameraController->onResize(m_ViewportSize.x, m_ViewportSize.y);
  }

  VEKTOR_CORE_TRACE("Delta time: {0} seconds", timestep.getSeconds());

  if (m_ViewportHovered && m_ViewportFocused)
  {
    m_CameraController->onUpdate(timestep);
  }

  m_Framebuffer->bind();

  glViewport(0, 0, (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
  glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  vektor::renderer::Renderer2D::beginScene(m_CameraController->getCamera());

  m_ActiveScene->onUpdate(timestep);

  vektor::renderer::Renderer2D::endScene();

  m_Framebuffer->unBind();

  VEKTOR_PROFILE_END_SESSION();
}

void EditorLayer::onRender()
{
  ImGui::Begin("Scene Hierarchy");

  for (auto e : m_Entities)
  {
    bool selected = (m_SelectedEntity == e);

    std::string tag = "Unnamed Entity";
    if (e.hasComponent<vektor::world::ecs::component_storage::TagComponent>())
      tag =
          e.getComponent<vektor::world::ecs::component_storage::TagComponent>()
              .getTag();

    if (ImGui::Selectable(
            (tag + " " + std::to_string((uint32_t)e.getEntity())).c_str(),
            selected))
      m_SelectedEntity = e;
  }

  ImGui::End();

  ImGui::Begin("Inspector");

  entt::registry &registry = m_ActiveScene->getRegistry();

  if (m_SelectedEntity.getEntity() != entt::null &&
      registry.valid(m_SelectedEntity.getEntity()))
  {
    if (m_SelectedEntity.hasComponent<
            vektor::world::ecs::component_storage::TransformComponent>())
    {
      auto &transform = m_SelectedEntity.getComponent<
          vektor::world::ecs::component_storage::TransformComponent>();

      auto &tagComponent = m_SelectedEntity.getComponent<
          vektor::world::ecs::component_storage::TagComponent>();
      std::string tag = tagComponent.getTag();

      char buffer[256];
      memset(buffer, 0, sizeof(buffer));
      strncpy(buffer, tag.c_str(), sizeof(buffer) - 1);

      ImGui::PushItemWidth(-1);

      if (ImGui::InputTextWithHint("##EntityName", "Entity Name", buffer,
                                   sizeof(buffer)))
      {
        tag = std::string(buffer);
        tagComponent.setTag(tag);
      }

      ImGui::PopItemWidth();

      ImGui::Text("Transform");
      ImGui::ColorEdit4("Color", glm::value_ptr(transform.color));
    }
  }

  ImGui::End();

  ImGui::Begin("Frame buffer viewport");
  m_ViewportFocused = ImGui::IsWindowFocused();
  m_ViewportHovered = ImGui::IsWindowHovered();

  ImVec2 viewportSize = ImGui::GetContentRegionAvail();
  m_ViewportSize = {viewportSize.x, viewportSize.y};

  uint32_t textureID = m_Framebuffer->getColorAttachmentRendererID();
  ImGui::Image((void *)(intptr_t)textureID,
               ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1),
               ImVec2(1, 0));

  ImGui::End();
}

void EditorLayer::onEvent(vektor::event::Event &event)
{
  if (m_ViewportHovered && m_ViewportFocused)
  {
    m_CameraController->onEvent(event);
  }
}