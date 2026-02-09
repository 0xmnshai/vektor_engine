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
        auto group = m_Registry.group<components::TransformComponent>();

        for (auto tranformEntity : group)
        {
            // auto &[transform, mesh] = group.get<components::TransformComponent, components::MeshComponent>(tranformEntity);
            auto &transform = group.get<components::TransformComponent>(tranformEntity);

            renderer::Renderer2D::drawQuad(transform.getLocalMatrix(), transform.getColor());
        }
    }
}