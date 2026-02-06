#pragma once

#include "vektor.hpp"

struct ParticleProps
{
    glm::vec2 position;
    glm::vec2 velocity, velocityVariation;
    glm::vec4 colorEnd, colorBegin;
    float sizeEnd, sizeBegin, sizeVariation;
    float lifeTime = 1.0f;
};

class ParticleLayer : public vektor::layer::Layer
{
public:
    ParticleLayer() : Layer("ParticleLayer") {}
    virtual void onAttach() override;

    void onUpdate(vektor::core::Timestep ts);
    void OnReader(vektor::renderer::camera::Orthographic &camera);
    void onEmit(const ParticleProps &props);

private:
    struct Particle
    {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 colorEnd, colorBegin;
        float sizeEnd, sizeBegin, sizeVariation;
        float lifeTime = 1.0f;
        float rotation = 0.0f;
        float lifeRemaining = 0.0f;
        bool active = false;
    };

    std::vector<Particle> m_Particles;
    uint32_t m_PoolIndex = 999;

    GLuint m_QuadVA = 0;
    std::unique_ptr<vektor::utils::Shader> m_ParticleShader;
    GLuint m_ParticleShaderViewProjection, m_ParticleShaderColor, m_ParticleShaderTranform;
};
