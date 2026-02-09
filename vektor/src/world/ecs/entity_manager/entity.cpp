#include "world/ecs/entity_manager/entity.hpp"

namespace vektor::world::ecs::entity_manager
{
    Entity::Entity(entt::entity entity, world::scene::Scene *scene)
        : m_Entity(entity), m_Scene(scene)
    {
    }
}