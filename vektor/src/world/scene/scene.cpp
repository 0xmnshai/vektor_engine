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
        auto group = m_Registry.group<world::ecs::component_storage::TransformComponent>();

        for (auto tranformEntity : group)
        {
            auto &transform = group.get<world::ecs::component_storage::TransformComponent>(tranformEntity);
            renderer::Renderer2D::drawQuad(transform.getLocalMatrix(), transform.getColor());
        }
    }
}