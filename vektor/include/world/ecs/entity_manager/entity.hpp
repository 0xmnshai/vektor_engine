#pragma once

#include "core/core.hpp"
#include "entt/entt.hpp"
#include "world/scene/scene.hpp"

namespace vektor::world::ecs::entity_manager
{
    class VEKTOR_API Entity
    {
    public:
        Entity(entt::entity entity, std::weak_ptr<world::scene::Scene> scene);
        Entity(const Entity &other) = default;
        Entity(Entity &&other) noexcept = default;
        Entity &operator=(const Entity &other) = default;
        Entity &operator=(Entity &&other) noexcept = default;

        ~Entity() = default;

        template <typename T, typename... Args>
        bool hasComponent() const
        {
            return m_Scene.lock()->hasEntity<T>();
        }

    private:
        entt::entity m_Entity;
        std::weak_ptr<world::scene::Scene> m_Scene;
    };
}