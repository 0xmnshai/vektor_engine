
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
}