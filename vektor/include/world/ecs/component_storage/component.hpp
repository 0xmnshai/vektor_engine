#pragma once

#include <vector>
#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

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

    private:
        void markDirty()
        {
            dirty = true;
            // children will be marked dirty by scene system
        }
    };
}
