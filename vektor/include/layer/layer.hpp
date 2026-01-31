#pragma once

#include <string>

#include "core/core.hpp"
#include "core/timestep.hpp"

#include "events/event.hpp"

namespace vektor::layer
{
    class VEKTOR_API Layer
    {
    public:
        Layer(const std::string &name = "Layer");
        virtual ~Layer();

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate(core::Timestep timestep) {}

        virtual void onRender() {}

        virtual void onEvent(event::Event &event) {}

        inline const std::string &getName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}