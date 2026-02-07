#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/core.hpp"
#include "utils/texture.hpp"

namespace vektor::utils
{
    class VEKTOR_API SubTexture2D
    {
    public:
        SubTexture2D(const std::shared_ptr<utils::Texture> &texture, const glm::vec2 &min, const glm::vec2 &max);

        inline const std::shared_ptr<utils::Texture> &getTexture() const { return m_Texture; }
        inline const glm::vec2 *getTextCoords() const { return m_TextCoords; }

        inline const glm::vec2 &getMin() const { return m_Min; }
        inline const glm::vec2 &getMax() const { return m_Max; }

        static std::shared_ptr<utils::SubTexture2D> createFromCords(const std::shared_ptr<utils::Texture> &texture, const glm::vec2 &coords, const glm::vec2 &spriteSize);

    private:
        std::shared_ptr<utils::Texture> m_Texture;
        glm::vec2 m_Min;
        glm::vec2 m_Max;

        glm::vec2 m_TextCoords[4];
    };
}