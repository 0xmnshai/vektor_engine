
#include <glad/glad.h>

#include "renderer/opengl.hpp"

namespace vektor::renderer
{
      void renderer::OpenGLAPI::setClearColor(const glm::vec4 &color)
      {
            glClearColor(color.r, color.g, color.b, color.a);
      };
      void renderer::OpenGLAPI::clear()
      {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      };

      void renderer::OpenGLAPI::drawIndexed(const std::shared_ptr<utils::VertexArray> &vertexArray)
      {
            vertexArray->bind();
            glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
      };
}