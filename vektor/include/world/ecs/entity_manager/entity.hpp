#pragma once

#include "core/core.hpp"
#include "world/scene/scene.hpp"

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
        bool hasComponent() const
        {
            return m_Scene->hasComponent<T>(m_Entity);
        }

        template <typename T, typename... Args>
        T &addComponent(Args &&...args)
        {
            VEKTOR_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
            return m_Scene->addComponent<T>(m_Entity, std::forward<Args>(args)...);
        }

        template <typename T>
        T &getComponent()
        {
            VEKTOR_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
            return m_Scene->getComponent<T>(m_Entity);
        }

        template <typename T>
        const T &getComponent() const
        {
            VEKTOR_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
            return m_Scene->getComponent<T>(m_Entity);
        }

        template <typename T>
        void removeComponent()
        {
            VEKTOR_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
            m_Scene->removeComponent<T>(m_Entity);
        }

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