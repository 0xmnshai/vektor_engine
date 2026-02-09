#include "scene/scene.hpp"

namespace vektor::scene
{
    Scene::Scene()
    {
        entt::entity entity = m_Registry.create();
        m_Registry.emplace<int>(entity, 42);
    }

    Scene::~Scene()
    {
    }

    void Scene::onUpdate(core::Timestep timestep)
    {
    }

    template <typename T, typename... Args>
    T &Scene::createEntity(Args &&...args)
    {
        entt::entity entity = m_Registry.create();
        m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
        return m_Registry.get<T>(entity);
    }
}