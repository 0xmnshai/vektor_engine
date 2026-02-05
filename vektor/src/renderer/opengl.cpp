
#include <glad/glad.h>

#include "renderer/opengl.hpp"

namespace vektor::renderer
{
      void renderer::OpenGLAPI::init()
      {
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      }

      void renderer::OpenGLAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
      {
            glViewport(x, y, width, height);
      };
      void renderer::OpenGLAPI::setClearColor(const glm::vec4 &color)
      {
            glClearColor(color.r, color.g, color.b, color.a);
      };
      void renderer::OpenGLAPI::clear()
      {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      };

      void renderer::OpenGLAPI::drawIndexed(const std::shared_ptr<utils::VertexArray> &vertexArray, uint32_t indexCount)
      {
            uint32_t count = indexCount == 0 ? vertexArray->getIndexBuffer()->getCount() : indexCount;
            vertexArray->bind();
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
      };
}