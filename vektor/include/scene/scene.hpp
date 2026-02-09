#pragma once

#include "entt/entt.hpp"

#include "core/core.hpp"
#include "core/timestep.hpp"

namespace vektor::scene
{
    class VEKTOR_API Scene
    {
    public:
        Scene();
        ~Scene();

        void onUpdate(core::Timestep timestep);

        template <typename T, typename... Args>
        T &createEntity(Args &&...args);

        template <typename T>
        inline bool hasEntity() const
        {
            return m_Registry.any_of<T>();
        }

    private:
        entt::registry m_Registry;
    };
}