#pragma once

#include <string>
#include <memory>
#include <cstdint>
#include <sstream>
#include <iostream>
#include <functional>
#include <type_traits>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#ifdef VEKTOR_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include "core/core.hpp"
#include "core/timestep.hpp"

#include "application/application.hpp"

#include "events/event.hpp"
#include "events/mouse.hpp"
#include "events/keycode.hpp"
#include "events/keyboard.hpp"
#include "events/mousebuttoncode.hpp"

#include "imgui/layer.hpp"

#include "input/input.hpp"
#include "input/window.hpp"

#include "layer/layer.hpp"
#include "layer/layer_stack.hpp"

#include "logger/logger.hpp"

#include "plugin/api.hpp"
#include "plugin/loader.hpp"
#include "plugin/plugin.hpp"
#include "plugin/shared_library.hpp"

#include "window/window.hpp"

#include "events/event.hpp"
#include "layer/layer.hpp"

#include "imgui/layer.hpp"

#include "input/input.hpp"
#include "input/window.hpp"

#include "utils/buffer.hpp"
#include "utils/vertex_array.hpp"

#include "opengl/shader.hpp"

#include "events/keycode.hpp"
#include "opengl/texture.hpp"
#include "utils/shader.hpp"

#include "renderer/2d.hpp"
#include "renderer/command.hpp"
#include "renderer/camera/orthographic.hpp"
#include "renderer/camera/controller.hpp"