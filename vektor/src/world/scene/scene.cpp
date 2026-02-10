#include "world/scene/scene.hpp"
#include "world/ecs/entity_manager/entity.hpp"
#include "world/ecs/component_storage/component.hpp"

namespace vektor::world::scene
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    world::ecs::entity_manager::Entity Scene::createEntity(std::string name)
    {
        world::ecs::entity_manager::Entity entity(m_Registry.create(), this);
        entity.addComponent<vektor::world::ecs::component_storage::TransformComponent>();
        auto &tag = entity.addComponent<vektor::world::ecs::component_storage::TagComponent>();
        tag.setTag(name);
        return entity;
    }

    void Scene::onViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // resize non fixed aspect ratio cameras
        auto cameraView = m_Registry.view<world::ecs::component_storage::CameraComponent>();
        cameraView.each([width, height](auto entity, world::ecs::component_storage::CameraComponent &camera)
                        {
                            if (!camera.fixedAspectRatio)
                            {
                                // camera.setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
                                camera.setViewPortSize(width, height);
                            } });
        }

    void Scene::onUpdate(core::Timestep timestep)
    {
        world::ecs::component_storage::CameraComponent *mainCamera = nullptr;
        world::ecs::component_storage::TransformComponent *cameraTransform = nullptr;
        bool found = false;

        auto cameraView = m_Registry.view<world::ecs::component_storage::TransformComponent, world::ecs::component_storage::CameraComponent>();

        cameraView.each([&](auto entity, world::ecs::component_storage::TransformComponent &transform, world::ecs::component_storage::CameraComponent &camera)
                        {
                            world::ecs::component_storage::CameraComponent &cam = cameraView.get<world::ecs::component_storage::CameraComponent>(entity);
                            world::ecs::component_storage::TransformComponent &tr = cameraView.get<world::ecs::component_storage::TransformComponent>(entity);

                            // if(camera.projectionType == world::ecs::component_storage::CameraComponent::CameraProjection::Perspective)
                            // {
                            //     renderer::Renderer2D::beginScene(cam, tr);
                            // }

                            if(found) return;

                            if (cam.isPrimary)
                            {
                                mainCamera = &cam;
                                cameraTransform = &tr;
                                found = true;
                            } });

        if (mainCamera)
        {
            renderer::Renderer2D::beginScene(*mainCamera, *cameraTransform);

            auto group = m_Registry.group<world::ecs::component_storage::TransformComponent>();

            for (auto entity : group)
            {
                if (m_Registry.any_of<world::ecs::component_storage::CameraComponent>(entity))
                    continue;

                auto &transform = group.get<world::ecs::component_storage::TransformComponent>(entity);
                renderer::Renderer2D::drawQuad(transform.getWorldMatrix(), transform.getColor());
            }

            renderer::Renderer2D::endScene();
        }
        // for (auto it = cameraView.begin(), end = cameraView.end(); it != end; ++it)
        // {
        //     entt::entity e = *it;
        //     auto &cam = cameraView.get<world::ecs::component_storage::CameraComponent>(e);
        //     auto &tr = cameraView.get<world::ecs::component_storage::TransformComponent>(e);
        // }

        // auto &camera = cameraView.get<world::ecs::component_storage::CameraComponent>(cameraEntity);
        // if (camera.projectionDirty)
        // {
        //     switch (camera.projectionType)
        //     {
        //     case world::ecs::component_storage::CameraComponent::CameraProjection::Perspective:
        //         camera.projectionMatrix = glm::perspective(glm::radians(camera.fovY), camera.aspectRatio, camera.nearPlane, camera.farPlane);
        //         break;
        //     case world::ecs::component_storage::CameraComponent::CameraProjection::Orthographic:
        //         camera.projectionMatrix = glm::ortho(-camera.orthoSize * camera.aspectRatio, camera.orthoSize * camera.aspectRatio, -camera.orthoSize, camera.orthoSize, camera.orthoNear, camera.orthoFar);
        //         break;
        //     default:
        //         break;
        //     }
        //     camera.projectionDirty = false;
        // }

        // if (camera.viewDirty)
        // {
        //     auto &transform = cameraView.get<world::ecs::component_storage::TransformComponent>(cameraEntity);
        //     camera.viewMatrix = glm::inverse(transform.getWorldMatrix());
        //     camera.viewProjectionMatrix = camera.projectionMatrix * camera.viewMatrix;
        //     camera.viewDirty = false;
        // }

        // auto group = m_Registry.group<world::ecs::component_storage::TransformComponent>();

        // for (auto tranformEntity : group)
        // {
        //     auto &transform = group.get<world::ecs::component_storage::TransformComponent>(tranformEntity);
        //     renderer::Renderer2D::drawQuad(transform.getLocalMatrix(), transform.getColor());
        // }

        // renderer::Renderer2D::endScene();
    }
}