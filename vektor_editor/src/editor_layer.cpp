#include "editor_layer.hpp"
#include "world/ecs/entity_manager/scriptable.hpp"

namespace entity_manager = vektor::world::ecs::entity_manager;

glm::vec3 clearColor = glm::vec3(0.1f, 0.1f, 0.1f);

EditorLayer::EditorLayer() : Layer("Editor Layer")
{
  float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
  m_CameraController = std::make_shared<vektor::renderer::camera::Controller>(aspectRatio);
  m_ActiveScene = std::make_shared<vektor::world::scene::Scene>();

  // main camera
  m_CameraObj = m_ActiveScene->createEntity("Camera");
  auto &cameraTransform = m_CameraObj.addComponent<vektor::world::ecs::component_storage::CameraComponent>();
  cameraTransform.setProjectionMatrix(glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f));
  cameraTransform.isPrimary = true;

  // clip space camera
  m_CameraObj2 = m_ActiveScene->createEntity("Clip Space Camera");
  auto &cameraComponent2 = m_CameraObj2.addComponent<vektor::world::ecs::component_storage::CameraComponent>();
  cameraComponent2.setProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
  cameraComponent2.isPrimary = false;

  m_PrimaryCamera = true;

  // TESTING NATIVE SCRIPTING ...

  class CameraController : public vektor::world::ecs::entity_manager::ScriptableEntity
  {
  public:
    void onCreate()
    {
      auto &cameraTransform = getComponent<vektor::world::ecs::component_storage::TransformComponent>();
      std::cout << "CameraController::onCreate" << std::endl;
    };
    void onUpdate(vektor::core::Timestep ts)
    {
      std::cout << "CameraController::onUpdate" << std::endl;

      auto &cameraTransform = getComponent<vektor::world::ecs::component_storage::TransformComponent>();
      float speed = 5.0f;

      auto &camera = getComponent<vektor::world::ecs::component_storage::CameraComponent>();

      if (!camera.isPrimary)
        return;

      if (vektor::input::Input::isKeyPressed(vektor::KeyCode::A))
        cameraTransform.translate({-speed * ts, 0.0f, 0.0f});

      if (vektor::input::Input::isKeyPressed(vektor::KeyCode::D))
        cameraTransform.translate({speed * ts, 0.0f, 0.0f});

      if (vektor::input::Input::isKeyPressed(vektor::KeyCode::W))
        cameraTransform.translate({0.0f, speed * ts, 0.0f});

      if (vektor::input::Input::isKeyPressed(vektor::KeyCode::S))
        cameraTransform.translate({0.0f, -speed * ts, 0.0f});
    };
    void onDestroy()
    {
      std::cout << "CameraController::onDestroy" << std::endl;
    };
  };

  m_CameraObj.addComponent<vektor::world::ecs::component_storage::NativeScriptComponent>().bindFunction<CameraController>();
}

void EditorLayer::onAttach()
{
  VEKTOR_CORE_INFO("EditorLayer layer attached");

  auto framebufferSpecification = vektor::renderer::FramebufferSpecification();
  framebufferSpecification.width = WINDOW_WIDTH;
  framebufferSpecification.height = WINDOW_HEIGHT;

  m_Framebuffer = vektor::renderer::Framebuffer::create(framebufferSpecification);

  for (int i = 0; i < 3; i++)
  {
    m_EntityObj = m_ActiveScene->createEntity("Square");
    auto &transform = m_EntityObj.getComponent<vektor::world::ecs::component_storage::TransformComponent>();

    transform.translate({i * 1.2f, 0.0f, 0.0f});
    transform.color = {1, 0, 0, 1};

    m_Entities.push_back(m_EntityObj);
  }
}
void EditorLayer::onDetach() { VEKTOR_CORE_INFO("EditorLayer layer detached"); }

void EditorLayer::onUpdate(vektor::core::Timestep timestep)
{
  VEKTOR_PROFILE_FUNCTION();

  VEKTOR_PROFILE_BEGIN_SESSION("EditorLayer::OnUpdate", "EditorLayer-Profile-Runtime.json");

  auto stats = m_Framebuffer->getSpecification();
  if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
      (stats.width != m_ViewportSize.x || stats.height != m_ViewportSize.y))
  {
    m_Framebuffer->resize((uint32_t)m_ViewportSize.x,
                          (uint32_t)m_ViewportSize.y);
    m_CameraController->onResize(m_ViewportSize.x, m_ViewportSize.y);

    m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x,
                                    (uint32_t)m_ViewportSize.y);
  }

  VEKTOR_CORE_TRACE("Delta time: {0} seconds", timestep.getSeconds());

  if (m_ViewportHovered && m_ViewportFocused)
  {
    // m_CameraController->onUpdate(timestep);

    // // TESTING ...
    // auto &activeCameraEntity = m_PrimaryCamera ? m_CameraObj : m_CameraObj2;
    // auto &tc = activeCameraEntity.getComponent<vektor::world::ecs::component_storage::TransformComponent>();

    // glm::mat4 mat = tc.getLocalMatrix();
    // mat[3] = glm::vec4(m_CameraController->getCameraPosition(), 1.0f);
    // tc.setLocalMatrix(mat);
  }

  m_Framebuffer->bind();

  glViewport(0, 0, (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
  glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // vektor::renderer::Renderer2D::beginScene(m_CameraController->getCamera());

  m_ActiveScene->onUpdate(timestep);

  // vektor::renderer::Renderer2D::endScene();

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
      tag = e.getComponent<vektor::world::ecs::component_storage::TagComponent>().getTag();

    if (ImGui::Selectable((tag + " " + std::to_string((uint32_t)e.getEntity())).c_str(), selected))
      m_SelectedEntity = e;
  }

  ImGui::End();

  ImGui::Begin("Inspector");

  entt::registry &registry = m_ActiveScene->getRegistry();

  if (m_SelectedEntity.getEntity() != entt::null &&
      registry.valid(m_SelectedEntity.getEntity()))
  {
    if (m_SelectedEntity.hasComponent<vektor::world::ecs::component_storage::TransformComponent>())
    {
      auto &transform = m_SelectedEntity.getComponent<vektor::world::ecs::component_storage::TransformComponent>();

      auto &tagComponent = m_SelectedEntity.getComponent<vektor::world::ecs::component_storage::TagComponent>();
      std::string tag = tagComponent.getTag();

      char buffer[256];
      memset(buffer, 0, sizeof(buffer));
      strncpy(buffer, tag.c_str(), sizeof(buffer) - 1);

      ImGui::PushItemWidth(-1);

      if (ImGui::InputTextWithHint("##EntityName", "Entity Name", buffer, sizeof(buffer)))
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

  // // clip space camera imgui
  // ImGui::DragFloat3("Clip camera Transform",
  //                   glm::value_ptr(m_CameraObj.getComponent<vektor::world::ecs::component_storage::TransformComponent>().localMatrix[3]));

  // // camera switch checkbox
  // if (ImGui::Checkbox("Primary Camera", &m_PrimaryCamera))
  // {
  //   m_CameraObj.getComponent<vektor::world::ecs::component_storage::CameraComponent>().isPrimary = m_PrimaryCamera;
  //   m_CameraObj2.getComponent<vektor::world::ecs::component_storage::CameraComponent>().isPrimary = !m_PrimaryCamera;
  // }

  ImGui::Begin("Camera Debug");

  auto &activeCameraEntity = m_PrimaryCamera ? m_CameraObj : m_CameraObj2;

  ImGui::Text("Currently Controlling: %s", m_PrimaryCamera ? "Main Camera" : "Clip Space Camera");

  auto &cameraTransform = activeCameraEntity.getComponent<vektor::world::ecs::component_storage::TransformComponent>();
  glm::vec3 pos = cameraTransform.getPosition();

  if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.1f))
  {
    glm::mat4 mat = cameraTransform.getLocalMatrix();
    mat[3] = glm::vec4(pos, 1.0f);
    cameraTransform.setLocalMatrix(mat);
  }

  ImGui::Separator();

  if (ImGui::Checkbox("Primary Camera", &m_PrimaryCamera))
  {
    m_CameraObj.getComponent<vektor::world::ecs::component_storage::CameraComponent>().isPrimary = m_PrimaryCamera;
    m_CameraObj2.getComponent<vektor::world::ecs::component_storage::CameraComponent>().isPrimary = !m_PrimaryCamera;

    // Optional: Sync the controller position to the new active camera so it doesn't "jump"
    auto &newTransform = activeCameraEntity.getComponent<vektor::world::ecs::component_storage::TransformComponent>();
    m_CameraController->setCameraPosition(newTransform.getPosition());
  }
  ImGui::End();

  // auto &cameraTransform = m_CameraObj.getComponent<vektor::world::ecs::component_storage::TransformComponent>();
  // glm::vec3 pos = cameraTransform.getPosition();

  // // Controls for the Clip Space Camera
  // ImGui::Text("Clip Space Camera");
  // // ImGui::DragFloat3("Position##Clip",
  // //                   glm::value_ptr(m_CameraObj.getComponent<vektor::world::ecs::component_storage::TransformComponent>().localMatrix[3]));

  // //   if (ImGui::DragFloat3("Position##Clip", glm::value_ptr(m_CameraObj.getComponent<...>().localMatrix[3]))) {
  // //     m_CameraObj.getComponent<...>().markDirty(); // Explicitly trigger the update
  // // }

  // if (ImGui::DragFloat3("Position##Clip", glm::value_ptr(pos), 0.1f))
  // {
  //   glm::mat4 mat = cameraTransform.getLocalMatrix();
  //   mat[3] = glm::vec4(pos, 1.0f);

  //   cameraTransform.setLocalMatrix(mat);
  // }

  // ImGui::Separator();

  // // Camera switch checkbox
  // if (ImGui::Checkbox("Primary Camera", &m_PrimaryCamera))
  // {
  //   m_CameraObj.getComponent<vektor::world::ecs::component_storage::CameraComponent>().isPrimary = m_PrimaryCamera;
  //   m_CameraObj2.getComponent<vektor::world::ecs::component_storage::CameraComponent>().isPrimary = !m_PrimaryCamera;
  // }
  // ImGui::End();

  ImGui::Begin("Frame buffer viewport");
  m_ViewportFocused = ImGui::IsWindowFocused();
  m_ViewportHovered = ImGui::IsWindowHovered();

  ImVec2 viewportSize = ImGui::GetContentRegionAvail();
  m_ViewportSize = {viewportSize.x, viewportSize.y};

  uint32_t textureID = m_Framebuffer->getColorAttachmentRendererID();
  ImGui::Image((void *)(intptr_t)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

  ImGui::End();
}

void EditorLayer::onEvent(vektor::event::Event &event)
{
  if (m_ViewportHovered && m_ViewportFocused)
  {
    m_CameraController->onEvent(event);
  }
}