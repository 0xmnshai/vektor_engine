#pragma once

#include "entt/entt.hpp"

#include "core/core.hpp"
// REMOVE: #include "world/scene/scene.hpp"

namespace vektor::world::scene
{
    class Scene;
}

namespace vektor::world::ecs::entity_manager
{
    class VEKTOR_API Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity entity, vektor::world::scene::Scene *scene);
        Entity(const Entity &other) = default;
        ~Entity() = default;

        template <typename T>
        bool hasComponent() const;

        template <typename T, typename... Args>
        T &addComponent(Args &&...args);

        template <typename T>
        T &getComponent();

        template <typename T>
        const T &getComponent() const;

        template <typename T>
        void removeComponent();

        operator entt::entity() const { return m_Entity; }
        operator bool() const { return m_Entity != entt::null; }
        entt::entity getEntity() const { return m_Entity; }

        bool operator==(const Entity &other) const
        {
            return m_Entity == other.m_Entity && m_Scene == other.m_Scene;
        }
        bool operator!=(const Entity &other) const
        {
            return !(*this == other);
        }

    private:
        entt::entity m_Entity{entt::null};
        vektor::world::scene::Scene *m_Scene = nullptr;
    };
}
