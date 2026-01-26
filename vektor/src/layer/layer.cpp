
#include "layer/layer.hpp"

namespace vektor::layer
{
    Layer::Layer(const std::string &name)
        : m_DebugName(name) {}

    Layer::~Layer() {}
}