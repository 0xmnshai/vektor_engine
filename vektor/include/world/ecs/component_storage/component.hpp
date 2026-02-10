#pragma once

#include <vector>
#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "core/core.hpp"
#include "core/timestep.hpp"
// #include "world/ecs/entity_manager/scriptable.hpp"

namespace vektor::world::ecs::entity_manager
{
    class ScriptableEntity;
}

namespace vektor::world::ecs::component_storage
{
    struct TagComponent
    {
        std::string tag;

        TagComponent() = default;

        TagComponent(const TagComponent &) = default;
        TagComponent(TagComponent &&) = default;

        TagComponent &operator=(const TagComponent &) = default;
        TagComponent &operator=(TagComponent &&) = default;

        TagComponent(const std::string &_tag)
            : tag(_tag) {}

        inline const std::string &getTag() const
        {
            return tag;
        }

        inline void setTag(const std::string &_tag)
        {
            tag = _tag;
        }

        inline bool operator==(const TagComponent &other) const
        {
            return tag == other.tag;
        }

        inline bool operator!=(const TagComponent &other) const
        {
            return tag != other.tag;
        }
    };

    struct TransformComponent
    {
        glm::mat4 localMatrix{1.0f};

        uint64_t parent = 0;            // EntityID
        std::vector<uint64_t> children; // EntityIDs

        mutable bool dirty = true;
        mutable glm::mat4 worldMatrix{1.0f};

        glm::vec4 color = glm::vec4(1.0f);

        TransformComponent() = default;

        TransformComponent(const TransformComponent &) = default;
        TransformComponent(TransformComponent &&) = default;

        TransformComponent &operator=(const TransformComponent &) = default;
        TransformComponent &operator=(TransformComponent &&) = default;

        TransformComponent(const glm::mat4 &matrix)
            : localMatrix(matrix), dirty(true) {}

        TransformComponent(const glm::vec3 &position,
                           const glm::quat &rotation,
                           const glm::vec3 &scale)
        {
            localMatrix =
                glm::translate(glm::mat4(1.0f), position) *
                glm::toMat4(rotation) *
                glm::scale(glm::mat4(1.0f), scale);
        }

        inline const glm::vec4 &getColor() const
        {
            return color;
        }

        inline void setColor(const glm::vec4 &_color)
        {
            color = _color;
        }

        const glm::mat4 &getLocalMatrix() const
        {
            return localMatrix;
        }

        const glm::mat4 &getWorldMatrix(const TransformComponent *parentTransform = nullptr) const
        {
            if (!dirty)
                return worldMatrix;

            if (parentTransform)
                worldMatrix = parentTransform->getWorldMatrix() * localMatrix;
            else
                worldMatrix = localMatrix;

            dirty = false;
            return worldMatrix;
        }

        void setLocalMatrix(const glm::mat4 &m)
        {
            localMatrix = m;
            markDirty();
        }

        void translate(const glm::vec3 &delta)
        {
            localMatrix = glm::translate(glm::mat4(1.0f), delta) * localMatrix;
            markDirty();
        }

        void rotate(const glm::quat &q)
        {
            localMatrix = glm::toMat4(q) * localMatrix;
            markDirty();
        }

        void scale(const glm::vec3 &s)
        {
            localMatrix = glm::scale(glm::mat4(1.0f), s) * localMatrix;
            markDirty();
        }

        glm::vec3 getPosition() const
        {
            glm::vec3 scale, translation, skew;
            glm::vec4 perspective;
            glm::quat rotation;
            glm::decompose(localMatrix, scale, rotation, translation, skew, perspective);
            return translation;
        }

        glm::quat getRotation() const
        {
            glm::vec3 scale, translation, skew;
            glm::vec4 perspective;
            glm::quat rotation;
            glm::decompose(localMatrix, scale, rotation, translation, skew, perspective);
            return rotation;
        }

        glm::vec3 getScale() const
        {
            glm::vec3 scale, translation, skew;
            glm::vec4 perspective;
            glm::quat rotation;
            glm::decompose(localMatrix, scale, rotation, translation, skew, perspective);
            return scale;
        }

        glm::vec3 forward() const
        {
            return glm::normalize(glm::vec3(getWorldMatrix()[2])) * -1.0f;
        }

        glm::vec3 right() const
        {
            return glm::normalize(glm::vec3(getWorldMatrix()[0]));
        }

        glm::vec3 up() const
        {
            return glm::normalize(glm::vec3(getWorldMatrix()[1]));
        }

    public:
        void markDirty()
        {
            dirty = true;
            // children will be marked dirty by scene system
        }
    };

    struct CameraComponent
    {
        enum class CameraProjection : uint8_t
        {
            Perspective = 0,
            Orthographic = 1
        };

        bool isPrimary = false;
        bool fixedAspectRatio = false;

        CameraProjection projectionType = CameraProjection::Perspective;

        float fovY = 60.0f; // degrees
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;

        float orthoSize = 10.0f; // half-height
        float orthoNear = -100.0f;
        float orthoFar = 100.0f;

        float aspectRatio = 1.0f;

        glm::mat4 projectionMatrix{1.0f};
        glm::mat4 viewMatrix{1.0f};
        glm::mat4 viewProjectionMatrix{1.0f};

        mutable bool projectionDirty = true;
        mutable bool viewDirty = true;

        CameraComponent() = default;

        CameraComponent(const CameraComponent &) = default;
        CameraComponent(CameraComponent &&) = default;

        CameraComponent &operator=(const CameraComponent &) = default;
        CameraComponent &operator=(CameraComponent &&) = default;

        CameraComponent(const glm::mat4 &projection, const glm::mat4 &view)
            : projectionMatrix(projection), viewMatrix(view), viewProjectionMatrix(projection * view) {}

        const glm::mat4 &getProjectionMatrix() const
        {
            return projectionMatrix;
        }

        const glm::mat4 &getViewMatrix() const
        {
            return viewMatrix;
        }

        const glm::mat4 &getViewProjectionMatrix() const
        {
            return viewProjectionMatrix;
        }

        void setViewPortSize(uint32_t width, uint32_t height)
        {
            aspectRatio = static_cast<float>(width) / static_cast<float>(height);

            // if (projectionType == CameraProjection::Perspective)
            // {
            //     projectionMatrix = glm::perspective(glm::radians(fovY), aspectRatio, nearPlane, farPlane);
            // }
            // else
            // {
            projectionMatrix = glm::ortho(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, orthoNear, orthoFar);
            // }

            viewProjectionMatrix = projectionMatrix * viewMatrix;
            projectionDirty = false;
            viewDirty = true;
        }

        void setPerspective(float fovDeg, float aspect, float nearP, float farP)
        {
            projectionType = CameraProjection::Perspective;
            fovY = fovDeg;
            aspectRatio = aspect;
            nearPlane = nearP;
            farPlane = farP;
            projectionDirty = true;
        }

        void setOrthographic(float size, float aspect, float nearP, float farP)
        {
            projectionType = CameraProjection::Orthographic;
            orthoSize = size;
            aspectRatio = aspect;
            orthoNear = nearP;
            orthoFar = farP;
            projectionDirty = true;
        }

        void setAspectRatio(float aspect)
        {
            aspectRatio = aspect;
            projectionDirty = true;
        }

        void setProjectionMatrix(const glm::mat4 &projection)
        {
            projectionMatrix = projection;
            viewProjectionMatrix = projection * viewMatrix;
            projectionDirty = false;
        }

        void setViewMatrix(const glm::mat4 &view)
        {
            viewMatrix = view;
            viewProjectionMatrix = projectionMatrix * view;
            viewDirty = false;
        }
    };

    struct NativeScriptComponent
    {
        world::ecs::entity_manager::ScriptableEntity *instance = nullptr;

        NativeScriptComponent() = default;
        NativeScriptComponent(const NativeScriptComponent &) = default;
        NativeScriptComponent(NativeScriptComponent &&) = default;
        NativeScriptComponent &operator=(const NativeScriptComponent &) = default;
        NativeScriptComponent &operator=(NativeScriptComponent &&) = default;

        NativeScriptComponent(uint64_t inst, world::ecs::entity_manager::ScriptableEntity *script)
            : instance(script) {}

        std::function<void()> instantiateFunction = nullptr;
        std::function<void()> destroyInstanceFunction = nullptr; // Corrected name

        std::function<void(world::ecs::entity_manager::ScriptableEntity *)> onCreateFunction = nullptr;
        std::function<void(world::ecs::entity_manager::ScriptableEntity *)> onDestroyFunction = nullptr;
        std::function<void(world::ecs::entity_manager::ScriptableEntity *, core::Timestep)> onUpdateFunction = nullptr;

        template <typename T>
        void bindFunction()
        {
            instantiateFunction = [&]()
            {
                instance = new T();
            };

            destroyInstanceFunction = [&]()
            {
                delete (T *)instance;
                instance = nullptr;
            };

            onCreateFunction = [](world::ecs::entity_manager::ScriptableEntity *inst)
            {
                ((T *)inst)->onCreate();
            };

            onDestroyFunction = [](world::ecs::entity_manager::ScriptableEntity *inst)
            {
                ((T *)inst)->onDestroy();
            };

            onUpdateFunction = [](world::ecs::entity_manager::ScriptableEntity *inst, core::Timestep ts)
            {
                ((T *)inst)->onUpdate(ts);
            };
        }

        inline world::ecs::entity_manager::ScriptableEntity *getInstance() const
        {
            return instance;
        }
    };

    // struct NativeScriptComponent
    // {
    //     world::ecs::entity_manager::ScriptableEntity *instance = nullptr;

    //     NativeScriptComponent() = default;

    //     NativeScriptComponent(const NativeScriptComponent &) = default;
    //     NativeScriptComponent(NativeScriptComponent &&) = default;

    //     NativeScriptComponent &operator=(const NativeScriptComponent &) = default;
    //     NativeScriptComponent &operator=(NativeScriptComponent &&) = default;

    //     NativeScriptComponent(uint64_t instance, world::ecs::entity_manager::ScriptableEntity *script)
    //         : instance(script) {}

    //     std::function<void()> instantiateFunction = nullptr;

    //     std::function<void()> destoryInstanceFunction = nullptr;

    //     std::function<void(world::ecs::entity_manager::ScriptableEntity *)> onCreateFunction = nullptr;
    //     std::function<void(world::ecs::entity_manager::ScriptableEntity *)> onDestroyFunction = nullptr;
    //     std::function<void(world::ecs::entity_manager::ScriptableEntity *, core::Timestep)> onUpdateFunction = nullptr;

    //     // template <typename T>
    //     // void bindCreateFunction()
    //     // {
    //     //     onCreateFunction = [instance = static_cast<T *>(instance)]()
    //     //     { instance->onCreate(); };
    //     // }

    //     template <typename T>
    //     void bindFunction()
    //     {
    //         // static_assert(std::is_base_of<world::ecs::entity_manager::ScriptableEntity, T>::value, "T must inherit from ScriptableEntity");

    //         instantiateFunction = [&instance = static_cast<T *>(instance)]()
    //         {
    //             instance = new T();
    //         };

    //         destoryInstanceFunction = [&instance = static_cast<T *>(instance)]()
    //         { delete (T *)instance; };

    //         onCreateFunction = [&](world::ecs::entity_manager::ScriptableEntity *instance)
    //         {
    //             ((T *)instance)->onCreate();
    //         };

    //         onDestroyFunction = [](world::ecs::entity_manager::ScriptableEntity *inst)
    //         {
    //             ((T *)inst)->onDestroy();
    //         };

    //         onUpdateFunction = [&](world::ecs::entity_manager::ScriptableEntity *instance, core::Timestep ts)
    //         {
    //             ((T *)instance)->onUpdate(ts);
    //         };
    //     }

    //     inline world::ecs::entity_manager::ScriptableEntity *getInstance() const
    //     {
    //         return instance;
    //     }

    // template <typename T>
    // void bindCreateFunction(T *instance)
    // {
    //     onCreateFunction = [instance]()
    //     { instance->onCreate(); };
    // }

    // template <typename T>
    // void bindUpdateFunction(T *instance)
    // {
    //     onUpdateFunction = [instance](core::Timestep ts)
    //     { instance->onUpdate(ts); };
    // }
};

/*

#pragma once
#include "scene/components.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace vektor::world::ecs
{
    class CameraSystem
    {
    public:
        static void Update(entt::registry& registry)
        {
            auto view = registry.view<
                component_storage::CameraComponent,
                component_storage::TransformComponent>();

            for (auto entity : view)
            {
                auto& cam = view.get<component_storage::CameraComponent>(entity);
                auto& transform = view.get<component_storage::TransformComponent>(entity);

                UpdateProjection(cam);
                UpdateView(cam, transform);
            }
        }

    private:
        static void UpdateProjection(component_storage::CameraComponent& cam)
        {
            if (!cam.projectionDirty)
                return;

            if (cam.projectionType == component_storage::CameraProjection::Perspective)
            {
                cam.projection = glm::perspective(
                    glm::radians(cam.fovY),
                    cam.aspectRatio,
                    cam.nearPlane,
                    cam.farPlane
                );
            }
            else
            {
                float h = cam.orthoSize;
                float w = h * cam.aspectRatio;

                cam.projection = glm::ortho(
                    -w, w,
                    -h, h,
                    cam.orthoNear,
                    cam.orthoFar
                );
            }

            cam.projectionDirty = false;
        }

        static void UpdateView(component_storage::CameraComponent& cam,
                               const component_storage::TransformComponent& transform)
        {
            if (!cam.viewDirty && !transform.dirty)
                return;

            glm::mat4 world = transform.getWorldMatrix();
            cam.view = glm::inverse(world);
            cam.viewProjection = cam.projection * cam.view;

            cam.viewDirty = false;
        }
    };
}


*/