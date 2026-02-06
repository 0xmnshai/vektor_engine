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

    vektor::renderer::Renderer2D::resetStats();

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
        vektor::renderer::Renderer2D::drawQuad({0.85f, 0.85f}, {10.0f, 10.5f}, m_Texture, tintColor);

        static float rotation = 0.0f;
        rotation += timestep.getSeconds() * 25.0f;

        vektor::renderer::Renderer2D::drawRotatedQuad({10.0f, 10.0f}, {1.0f, 1.0f}, rotation, m_Texture, m_Color);

        // for (float y = -5.0f; y < 10.0f; y += 0.5f)
        // {
        //     for (float x = -5.0f; x < 10.0f; x += 0.5f)
        //     {
        //         glm::vec4 color = {((x + 5.0f) / 10.0f), 0.0f, ((y + 5.0f) / 10.0f), 1.0f};
        //         vektor::renderer::Renderer2D::drawQuad({x, y}, {0.45f, 0.45f}, color);
        //     }
        // }

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

    ImGui::Begin("Stats");

    auto stats = vektor::renderer::Renderer2D::getStats();
    ImGui::Text("Draw Calls: %d", stats.getDrawCalls());
    ImGui::Text("Quad Count: %d", stats.getQuadCount());
    ImGui::Text("Total Vertex Count: %d", stats.getTotalVertexCount());
    ImGui::Text("Total Index Count: %d", stats.getTotalIndexCount());

    ImGui::End();
}

void Sandbox2D::onEvent(vektor::event::Event &event)
{
    m_CameraController->onEvent(event);
}