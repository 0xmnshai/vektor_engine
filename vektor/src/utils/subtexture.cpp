
#include "utils/subtexture.hpp"

namespace vektor::utils
{
    SubTexture2D::SubTexture2D(const std::shared_ptr<utils::Texture> &texture, const glm::vec2 &min, const glm::vec2 &max)
        : m_Texture(texture)
    {
        m_Min = min;
        m_Max = max;

        m_TextCoords[0] = {min.x, min.y};
        m_TextCoords[1] = {max.x, min.y};
        m_TextCoords[2] = {max.x, max.y};
        m_TextCoords[3] = {min.x, max.y};
    }

    std::shared_ptr<utils::SubTexture2D> SubTexture2D::createFromCords(const std::shared_ptr<utils::Texture> &texture, const glm::vec2 &coords, const glm::vec2 &spriteSize)
    {
        glm::vec2 min = {(coords.x * spriteSize.x) / texture->getWidth(), (coords.y * spriteSize.y) / texture->getHeight()};
        glm::vec2 max = {((coords.x + 1) * spriteSize.x) / texture->getWidth(), ((coords.y + 1) * spriteSize.y) / texture->getHeight()};

        return std::make_shared<SubTexture2D>(texture, min, max);
    }
}
