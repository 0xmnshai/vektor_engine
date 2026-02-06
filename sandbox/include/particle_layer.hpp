#pragma once

#include "vektor.hpp"

struct ParticleProps
{
    glm::vec2 position;
    glm::vec2 velocity, velocityVariation;
    glm::vec4 colorBegin, colorEnd;
    float sizeBegin, sizeEnd, sizeVariation;
    float lifeTime = 1.0f;
};

class ParticleSystem
{
public:
    ParticleSystem(uint32_t maxParticles = 1000);

    void onUpdate(vektor::core::Timestep ts);
    void onRender();

    void emit(const ParticleProps &particleProps);

private:
    struct Particle
    {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 colorBegin, colorEnd;
        float rotation = 0.0f;
        float sizeBegin, sizeEnd;

        float lifeTime = 1.0f;
        float lifeRemaining = 0.0f;

        bool active = false;
    };

    std::vector<Particle> m_ParticlePool;
    uint32_t m_PoolIndex = 999;
};

class ParticleLayer : public vektor::layer::Layer
{
public:
    ParticleLayer();
    virtual ~ParticleLayer() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;

    virtual void onUpdate(vektor::core::Timestep ts) override;
    virtual void onRender() override;
    virtual void onEvent(vektor::event::Event &event) override;

private:
    std::shared_ptr<vektor::renderer::camera::Controller> m_CameraController;
    ParticleSystem m_ParticleSystem;
    ParticleProps m_ParticleProps;
};