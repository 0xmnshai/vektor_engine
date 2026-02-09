#pragma once

#include "entt/entt.hpp"

#include "core/core.hpp"
#include "core/timestep.hpp"

#include "renderer/api.hpp"
#include "renderer/2d.hpp"

#include "world/component_storage/component.hpp"

namespace vektor::world::scene
{
    class VEKTOR_API Scene
    {
    public:
        Scene();
        ~Scene();

        void onUpdate(core::Timestep timestep);

        // template <typename T, typename... Args>
        // inline entt:entity &createEntity(Args &&...args)
        // {
        //     // entt::entity entity = m_Registry.create();
        //     // m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
        //     // return m_Registry.get<T>(entity);

        //     return m_Registry.create();
        // }

        inline entt::entity createEntity()
        {
            return m_Registry.create();
        }

        template <typename T, typename... Args>
        inline T &addComponent(entt::entity e, Args &&...args)
        {
            return m_Registry.emplace<T>(e, std::forward<Args>(args)...);
        }

        template <typename T>
        inline T &getComponent(entt::entity e)
        {
            return m_Registry.get<T>(e);
        }

        template <typename T>
        inline bool hasEntity() const
        {
            return m_Registry.any_of<T>();
        }

        entt::registry &getRegistry() { return m_Registry; }

    private:
        entt::registry m_Registry;
    };
}