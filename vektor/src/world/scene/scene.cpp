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

    void Scene::onUpdate(core::Timestep timestep)
    {

        auto cameraView = m_Registry.view<
            world::ecs::component_storage::TransformComponent,
            world::ecs::component_storage::CameraComponent>();

        cameraView.each([=](auto entity, auto &transform, auto &camera)
                        {
                            world::ecs::component_storage::CameraComponent &cam = cameraView.get<world::ecs::component_storage::CameraComponent>(entity);
                            world::ecs::component_storage::TransformComponent &tr = cameraView.get<world::ecs::component_storage::TransformComponent>(entity);
 

                            if(camera.projectionType::CameraProjection::Perspective)
                            {
                                
                            }
                        });

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

        auto group = m_Registry.group<world::ecs::component_storage::TransformComponent>();

        for (auto tranformEntity : group)
        {
            auto &transform = group.get<world::ecs::component_storage::TransformComponent>(tranformEntity);
            renderer::Renderer2D::drawQuad(transform.getLocalMatrix(), transform.getColor());
        }
    }
}