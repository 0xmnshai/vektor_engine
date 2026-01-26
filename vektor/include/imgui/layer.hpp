#pragma once

#include "core/core.hpp"
#include "layer/layer.hpp"

namespace vektor::imgui_layer
{
    class VEKTOR_API Layer : public layer::Layer
    {
    public:
        Layer();
        ~Layer();

        void onAttach() override;
        void onDetach() override;
        void onUpdate() override;
        void onEvent(event::Event &event) override;

    private:
        bool m_BlockEvents = true;
        float m_Time = 0.0f;
    };
}