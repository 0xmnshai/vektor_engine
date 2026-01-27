
#include "vk_pch.hpp"
#include "layer/layer_stack.hpp"

namespace vektor::layer
{
    LayerStack::LayerStack()
    {
        // m_layerInsertIndex = m_layers.begin();
    }

    LayerStack::~LayerStack()
    {
        for (Layer *layer : m_layers)
        {
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer *layer)
    {
        // m_layers.emplace(m_layerInsertIndex, layer);
        // m_layerInsertIndex++;

        m_layers.insert(m_layers.begin() + m_layerInsertIndex, layer);
        m_layerInsertIndex++;
    }

    void LayerStack::popLayer(Layer *layer)
    {
        // auto it = std::find(m_layers.begin(), m_layers.end(), layer);
        // if (it != m_layers.end())
        // {
        //     m_layers.erase(it);
        //     m_layerInsertIndex--;
        // }

        auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
        if (it != m_layers.begin() + m_layerInsertIndex)
        {
            m_layers.erase(it);
            m_layerInsertIndex--;
        }
    }

    void LayerStack::pushOverlay(Layer *overlay)
    {
        m_layers.emplace_back(overlay);
    }

    void LayerStack::popOverlay(Layer *overlay)
    {
        // auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
        // if (it != m_layers.end())
        // {
        //     m_layers.erase(it);
        // }

        auto it = std::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), overlay);
        if (it != m_layers.end())
            m_layers.erase(it);
    }
}