#pragma once

#include "entt/entt.hpp"

#include "core/core.hpp"
#include "core/timestep.hpp"

#include "renderer/api.hpp"
#include "renderer/2d.hpp"

#include "scene/components.hpp"

namespace vektor::scene
{
    class VEKTOR_API Scene
    {
    public:
        Scene();
        ~Scene();

        void onUpdate(core::Timestep timestep);

        template <typename T, typename... Args>
        inline T &createEntity(Args &&...args)
        {
            entt::entity entity = m_Registry.create();
            m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
            return m_Registry.get<T>(entity);
        }

        template <typename T>
        inline bool hasEntity() const
        {
            return m_Registry.any_of<T>();
        }

    private:
        entt::registry m_Registry;
    };
}