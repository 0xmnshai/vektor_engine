// Helper macros to force expansion of __LINE__
#define PROFILE_CONCAT_INNER(x, y) x##y
#define PROFILE_CONCAT(x, y) PROFILE_CONCAT_INNER(x, y)

#define SANDBOX_PROFILE_SCOPE(name)                                                                 \
    vektor::utils::Timer PROFILE_CONCAT(timer, __LINE__)(name,                                      \
                                                         [&](const vektor::utils::ProfileResult &r) \
                                                         {                                          \
                                                             m_ProfileResults.push_back(r);         \
                                                         });                                        \
    PROFILE_CONCAT(timer, __LINE__).start();

#include "sandbox2d.hpp"

#include "sandbox2d.hpp"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D")
{
    float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
    m_CameraController = std::make_shared<vektor::renderer::camera::Controller>(aspectRatio);
}

void Sandbox2D::onAttach()
{
    VEKTOR_CORE_INFO("Sandbox2D layer attached");

    m_Texture = vektor::utils::Texture::create("/Users/lazycodebaker/Documents/Dev/CPP/vektor_engine/assets/image.png");
}

void Sandbox2D::onDetach()
{
    VEKTOR_CORE_INFO("Sandbox2D layer detached");
}

void Sandbox2D::onUpdate(vektor::core::Timestep timestep)
{
    VEKTOR_PROFILE_FUNCTION();

    VEKTOR_PROFILE_BEGIN_SESSION("Sandbox2D::OnUpdate", "SandboxUpdateProfile-Runtime.json");

    SANDBOX_PROFILE_SCOPE("Sandbox2D::OnUpdate");

    VEKTOR_CORE_TRACE("Delta time: {0} seconds", timestep.getSeconds());

    {
        SANDBOX_PROFILE_SCOPE("Sandbox2D::OnUpdate::CameraController::OnUpdate");
        m_CameraController->onUpdate(timestep);
    }

    {
        SANDBOX_PROFILE_SCOPE("Sandbox2D::OnUpdate::Renderer::OnRender");
        vektor::renderer::Command::setClearColor({0.13f, 0.13f, 0.13f, 1.0f}); // normalised: {value} / 255
        vektor::renderer::Command::clear();
    }
    {
        SANDBOX_PROFILE_SCOPE("Sandbox2D::OnUpdate::Renderer2D::OnRender");
        vektor::renderer::Renderer2D::beginScene(m_CameraController->getCamera());

        vektor::renderer::Renderer2D::drawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, m_Color);
        vektor::renderer::Renderer2D::drawQuad({0.5f, 0.5f}, {0.5f, 1.5f}, {0.8f, 0.2f, 0.3f, 1.0f});

        // blue 
        const glm::vec4 tintColor = {0.2f, 0.5f, 0.8f, 1.0f};
 
        vektor::renderer::Renderer2D::drawRotatedQuad({-0.5f, 0.5f}, {1.0f, 1.0f}, 45.0f, m_Texture, tintColor);

        m_Texture->bind();
        vektor::renderer::Renderer2D::drawQuad({0.85f, 0.85f}, {10.0f, 10.5f}, m_Texture, tintColor);
        vektor::renderer::Renderer2D::endScene();
    }
    VEKTOR_PROFILE_END_SESSION();
}

void Sandbox2D::onRender()
{
    ImGui::Begin("Color Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_Color));

    for (auto &profileResult : m_ProfileResults)
    {
        ImGui::Text("%s: %lld us",
                    profileResult.name.c_str(),
                    profileResult.end - profileResult.start);
    };

    m_ProfileResults.clear();
    ImGui::End();
}

void Sandbox2D::onEvent(vektor::event::Event &event)
{
    m_CameraController->onEvent(event);
}