#pragma once

#include "core/core.hpp"
#include "world/scene/scene.hpp"
#include "core/timestep.hpp"

namespace vektor::world::ecs::entity_manager
{
    class VEKTOR_API ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() {}

        template <typename T>
        T &getComponent()
        {
            return m_Entity.getComponent<T>();
        }

        virtual void onCreate() {}
        virtual void onDestroy() {}
        virtual void onUpdate(core::Timestep ts) {}

    private:
        world::ecs::entity_manager::Entity m_Entity;
        friend class vektor::world::scene::Scene;
    };
}