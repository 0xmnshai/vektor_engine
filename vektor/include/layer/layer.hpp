#pragma once

#include <string>

#include "events/event.hpp"
#include "core/core.hpp"

namespace vektor::layer
{
    class VEKTOR_API Layer
    {
    public:
        Layer(const std::string &name = "Layer");
        virtual ~Layer();

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate() {}
        
        virtual void onRender() {}
        
        virtual void onEvent(event::Event &event) {}

        inline const std::string &getName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}