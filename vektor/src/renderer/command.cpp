
#include "renderer/api.hpp"
#include "renderer/command.hpp"
#include "renderer/opengl.hpp"

namespace vektor::renderer
{
    renderer::RendererApi *Command::s_RendererAPI = new renderer::OpenGLAPI();
}