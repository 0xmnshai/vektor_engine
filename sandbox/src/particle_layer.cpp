#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include <random>

#include "particle_layer.hpp"

class Random
{
public:
    static void init()
    {
        s_RandomEngine.seed(std::random_device()());
    }

    static float Float()
    {
        return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
    }

private:
    static std::mt19937 s_RandomEngine;
    static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

ParticleSystem::ParticleSystem(uint32_t maxParticles)
{
    m_ParticlePool.resize(maxParticles);
    m_PoolIndex = maxParticles - 1;
}

void ParticleSystem::onUpdate(vektor::core::Timestep ts)
{
    for (auto &particle : m_ParticlePool)
    {
        if (!particle.active)
            continue;

        if (particle.lifeRemaining <= 0.0f)
        {
            particle.active = false;
            continue;
        }

        particle.lifeRemaining -= ts;
        particle.position += particle.velocity * (float)ts;
        particle.rotation += 0.01f * ts;
    }
}

void ParticleSystem::onRender()
{
    for (auto &particle : m_ParticlePool)
    {
        if (!particle.active)
            continue;

        float life = particle.lifeRemaining / particle.lifeTime;

        glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
        float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);

        vektor::renderer::Renderer2D::drawRotatedQuad(
            {particle.position.x, particle.position.y},
            {size, size},
            particle.rotation,
            color);
    }
}

void ParticleSystem::emit(const ParticleProps &particleProps)
{
    Particle &particle = m_ParticlePool[m_PoolIndex];

    particle.active = true;
    particle.position = particleProps.position;
    particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

    particle.velocity = particleProps.velocity;
    particle.velocity.x += particleProps.velocityVariation.x * (Random::Float() - 0.5f);
    particle.velocity.y += particleProps.velocityVariation.y * (Random::Float() - 0.5f);

    particle.colorBegin = particleProps.colorBegin;
    particle.colorEnd = particleProps.colorEnd;

    particle.lifeTime = particleProps.lifeTime;
    particle.lifeRemaining = particleProps.lifeTime;

    particle.sizeBegin = particleProps.sizeBegin + particleProps.sizeVariation * (Random::Float() - 0.5f);
    particle.sizeEnd = particleProps.sizeEnd;

    m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}

ParticleLayer::ParticleLayer()
    : Layer("ParticleLayer"), m_ParticleSystem(1000)
{
    float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    m_CameraController = std::make_shared<vektor::renderer::camera::Controller>(aspectRatio);
    m_CameraController->setZoomLevel(5.0f);
}

void ParticleLayer::onAttach()
{
    Random::init();

    m_ParticleProps.colorBegin = {254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f};
    m_ParticleProps.colorEnd = {254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f};
    m_ParticleProps.sizeBegin = 0.5f;
    m_ParticleProps.sizeVariation = 0.3f;
    m_ParticleProps.sizeEnd = 0.0f;
    m_ParticleProps.lifeTime = 1.0f;
    m_ParticleProps.velocity = {0.0f, 0.0f};
    m_ParticleProps.velocityVariation = {3.0f, 1.0f};
    m_ParticleProps.position = {0.0f, 0.0f};
}

void ParticleLayer::onDetach() {}

void ParticleLayer::onUpdate(vektor::core::Timestep ts)
{
    m_CameraController->onUpdate(ts);

    if (vektor::input::Input::isMouseButtonPressed(vektor::MouseCode::Left))
    {
        glm::vec2 mousePosition = vektor::input::Input::getMousePosition();

        float x = mousePosition.x;
        float y = mousePosition.y;

        auto nativeWindow = static_cast<GLFWwindow *>(vektor::Application::getInstance().getWindow().getNativeWindow());
        int winWidth, winHeight;
        glfwGetWindowSize(nativeWindow, &winWidth, &winHeight);

        float x_ndc = (x / (float)winWidth) * 2.0f - 1.0f;
        float y_ndc = -((y / (float)winHeight) * 2.0f - 1.0f);
        float aspect = (float)winWidth / (float)winHeight;

        auto camPos = m_CameraController->getCameraPosition();
        float zoom = m_CameraController->getZoomLevel();

        m_ParticleProps.position.x = camPos.x + (x_ndc * aspect * zoom);
        m_ParticleProps.position.y = camPos.y + (y_ndc * zoom);

        for (int i = 0; i < 5; i++)
            m_ParticleSystem.emit(m_ParticleProps);
    }

    m_ParticleSystem.onUpdate(ts);

    vektor::renderer::Command::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    vektor::renderer::Command::clear();

    vektor::renderer::Renderer2D::beginScene(m_CameraController->getCamera());
    m_ParticleSystem.onRender();
    vektor::renderer::Renderer2D::endScene();
}

void ParticleLayer::onRender()
{
    ImGui::Begin("Particle Settings");
    ImGui::ColorEdit4("Color Begin", glm::value_ptr(m_ParticleProps.colorBegin));
    ImGui::ColorEdit4("Color End", glm::value_ptr(m_ParticleProps.colorEnd));
    ImGui::SliderFloat("Size Begin", &m_ParticleProps.sizeBegin, 0.0f, 10.0f);
    ImGui::SliderFloat("Size End", &m_ParticleProps.sizeEnd, 0.0f, 10.0f);
    ImGui::SliderFloat("Size Variation", &m_ParticleProps.sizeVariation, 0.0f, 1.0f);
    ImGui::SliderFloat("Life Time", &m_ParticleProps.lifeTime, 0.1f, 10.0f);
    ImGui::End();
}

void ParticleLayer::onEvent(vektor::event::Event &event)
{
    m_CameraController->onEvent(event);
}