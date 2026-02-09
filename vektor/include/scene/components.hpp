#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/core.hpp"

namespace vektor::components
{
    struct TransformComponent
    {
        glm::mat4 transform = glm::mat4(1.0f);

        TransformComponent() = default;
        TransformComponent(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
            : transform(glm::translate(glm::mat4(1.0f), position) * glm::toMat4(glm::quat(rotation)) * glm::scale(glm::mat4(1.0f), scale))
        {
        }

        TransformComponent(const TransformComponent &) = default;
        TransformComponent(TransformComponent &&) = default;
        ~TransformComponent() = default;

        TransformComponent(const glm::mat4 &transform)
            : transform(transform)
        {
        }

        operator glm::mat4 &() { return transform; }
        operator const glm::mat4 &() const { return transform; }
    };

}