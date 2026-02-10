#pragma once

#include "entt/entt.hpp"

#include "core/core.hpp"
#include "core/timestep.hpp"

#include "renderer/api.hpp"
#include "renderer/2d.hpp"

#include "world/ecs/component_storage/component.hpp"
#include "world/ecs/entity_manager/entity.hpp"

namespace vektor::world::scene
{
    class VEKTOR_API Scene
    {
    public:
        Scene();
        ~Scene();

        void onUpdate(core::Timestep timestep);

        world::ecs::entity_manager::Entity createEntity(std::string name = "Unnamed Entity");

        template <typename T, typename... Args>
        T &addComponent(entt::entity e, Args &&...args)
        {
            return m_Registry.emplace<T>(e, std::forward<Args>(args)...);
        }

        template <typename T>
        T &getComponent(entt::entity e)
        {
            return m_Registry.get<T>(e);
        }

        template <typename T>
        bool hasComponent(entt::entity e) const
        {
            return m_Registry.all_of<T>(e);
        }

        template <typename T>
        void removeComponent(entt::entity entity)
        {
            m_Registry.remove<T>(entity);
        }

        entt::registry &getRegistry() { return m_Registry; }

        void onViewportResize(uint32_t width, uint32_t height);

    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
    };
}

namespace vektor::world::ecs::entity_manager
{
    template <typename T>
    bool Entity::hasComponent() const
    {
        return m_Scene->hasComponent<T>(m_Entity);
    }

    template <typename T, typename... Args>
    T &Entity::addComponent(Args &&...args)
    {
        VEKTOR_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
        return m_Scene->addComponent<T>(m_Entity, std::forward<Args>(args)...);
    }

    template <typename T>
    T &Entity::getComponent()
    {
        VEKTOR_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
        return m_Scene->getComponent<T>(m_Entity);
    }

    template <typename T>
    const T &Entity::getComponent() const
    {
        VEKTOR_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
        return m_Scene->getComponent<T>(m_Entity);
    }

    template <typename T>
    void Entity::removeComponent()
    {
        VEKTOR_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
        m_Scene->removeComponent<T>(m_Entity);
    }
}