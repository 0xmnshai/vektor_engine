#include "utils/buffer.hpp"
#include "opengl/buffer.hpp"

// In a real engine, you would check a Renderer::GetAPI() here
// For now, we assume OpenGL is the only backend.

namespace vektor::utils::buffer
{
    Vertex *Vertex::create(const std::vector<float> &vertices)
    {
        return new opengl::VertexBuffer(vertices);
    }

    Index *Index::create(const std::vector<uint32_t> &indices)
    {
        return new opengl::IndexBuffer(indices);
    }
}