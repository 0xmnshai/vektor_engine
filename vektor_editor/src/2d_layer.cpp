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

#include "2d_layer.hpp"

VektorEditor2D::VektorEditor2D()
    : Layer("VektorEditor2D")
{
    float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
    m_CameraController = std::make_shared<vektor::renderer::camera::Controller>(aspectRatio);
}

void VektorEditor2D::onAttach()
{
    VEKTOR_CORE_INFO("2D layer attached");

    m_Texture = vektor::utils::Texture::create("/Users/lazycodebaker/Documents/Dev/CPP/vektor_engine/assets/image.png");

    vektor::renderer::FramebufferSpecification spec;
    spec.width = WINDOW_WIDTH;
    spec.height = WINDOW_HEIGHT;
    spec.samples = 1;
    spec.colorAttachmentFormat = vektor::renderer::FramebufferFormat::TextureFormat::RGBA8;
    spec.depthAttachmentFormat = vektor::renderer::FramebufferFormat::TextureFormat::DEPTH24STENCIL8;

    m_Framebuffer = vektor::renderer::Framebuffer::create(spec);
}

void VektorEditor2D::onDetach()
{
    VEKTOR_CORE_INFO("2D layer detached");
}

void VektorEditor2D::onUpdate(vektor::core::Timestep timestep)
{
    VEKTOR_PROFILE_FUNCTION();

    VEKTOR_PROFILE_BEGIN_SESSION("VektorEditor2D::OnUpdate", "VektorEditorUpdateProfile-Runtime.json");

    SANDBOX_PROFILE_SCOPE("VektorEditor2D::OnUpdate");

    VEKTOR_CORE_TRACE("Delta time: {0} seconds", timestep.getSeconds());

    vektor::renderer::Renderer2D::resetStats();

    m_Framebuffer->bind();

    {
        if (m_ViewportFocused)
        {
            SANDBOX_PROFILE_SCOPE("VektorEditor2D::OnUpdate::CameraController::OnUpdate");
            m_CameraController->onUpdate(timestep);
        }
    }

    {
        SANDBOX_PROFILE_SCOPE("VektorEditor2D::OnUpdate::Renderer::OnRender");
        vektor::renderer::Command::setClearColor({0.13f, 0.13f, 0.13f, 1.0f}); // normalised: {value} / 255
        vektor::renderer::Command::clear();
    }

    {
        SANDBOX_PROFILE_SCOPE("VektorEditor2D::OnUpdate::Renderer2D::OnRender");
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

        vektor::renderer::Renderer2D::endScene();
    }

    m_Framebuffer->unBind();

    VEKTOR_PROFILE_END_SESSION();
}

void VektorEditor2D::onRender()
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

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    if (m_ViewportSize != *reinterpret_cast<glm::vec2 *>(&viewportSize) && (viewportSize.x > 0.0f && viewportSize.y > 0.0f))
    {
        m_Framebuffer->resize(viewportSize.x, viewportSize.y);
        m_ViewportSize = *reinterpret_cast<glm::vec2 *>(&viewportSize);

        m_CameraController->onResize(viewportSize.x, viewportSize.y);
    }

    uint32_t textureID = m_Framebuffer->getColorAttachmentRendererID();
    ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();

    static bool dockspaceOpen = true;
    static bool fullscreenPersistant = true;
    bool fullscreen = fullscreenPersistant;

    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoDocking;

    if (fullscreen)
    {
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        windowFlags |=
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        windowFlags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("VektorDockspace", &dockspaceOpen, windowFlags);

    ImGui::PopStyleVar(); // padding

    if (fullscreen)
        ImGui::PopStyleVar(2);

    if (!ImGui::BeginMenuBar())
        return;

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Scene", "Ctrl+N"))
        {
        }
        if (ImGui::MenuItem("Open...", "Ctrl+O"))
        {
        }
        if (ImGui::MenuItem("Save", "Ctrl+S"))
        {
        }
        if (ImGui::MenuItem("Exit"))
        {
            std::cout << "Exiting..." << std::endl;
            exit(0);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit"))
    {
        ImGui::MenuItem("Undo", "Ctrl+Z");
        ImGui::MenuItem("Redo", "Ctrl+Y");
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window"))
    {
        ImGui::MenuItem("Scene Hierarchy");
        ImGui::MenuItem("Inspector");
        ImGui::MenuItem("Console");
        ImGui::EndMenu();
    }

    ImGui::EndMenuBar();

    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspaceID = ImGui::GetID("VektorDockspaceID");
        ImGui::DockSpace(dockspaceID, ImVec2(0, 0), dockspaceFlags);
    }

    ImGui::End();
}

void VektorEditor2D::onEvent(vektor::event::Event &event)
{
    if (m_ViewportHovered)
    {
        m_CameraController->onEvent(event);
    }
}