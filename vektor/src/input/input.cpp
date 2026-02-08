#include "input/window.hpp"

namespace vektor::input
{
    Input *Input::s_Instance = new window::Input();

#ifdef VEKTOR_PLATFORM_WINDOWS
#include "input/window.hpp"
    std::shared_ptr<input::window::Input> Input::create()
    {
        return std::make_shared<input::window::Input>();
    }
#endif
}
