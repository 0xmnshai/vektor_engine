#include <imgui.h>

#include "game_layer.hpp"

GameLayer::GameLayer() : Layer("GameLayer")
{
    float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    m_CameraController = std::make_shared<vektor::renderer::camera::Controller>(aspectRatio);
    m_CameraController->setZoomLevel(5.0f);
}

void GameLayer::onAttach()
{
    resetGame();
}

void GameLayer::resetGame()
{
    m_GameOver = false;
    m_Score = 0;

    m_Player.Position = {0.0f, 2.0f};
    m_Player.Size = {0.8f, 0.8f};
    m_Player.Color = {0.0f, 0.8f, 0.2f, 1.0f};
    m_PlayerVelocity = {0.0f, 0.0f};

    m_LevelData.clear();

    m_LevelData.push_back({{0.0f, -2.0f}, {20.0f, 1.0f}, {0.3f, 0.3f, 0.3f, 1.0f}});

    m_LevelData.push_back({{-3.0f, 0.5f}, {2.0f, 0.4f}, {0.6f, 0.4f, 0.2f, 1.0f}});
    m_LevelData.push_back({{3.0f, 1.5f}, {2.0f, 0.4f}, {0.6f, 0.4f, 0.2f, 1.0f}});
    m_LevelData.push_back({{0.0f, 3.5f}, {3.0f, 0.4f}, {0.6f, 0.4f, 0.2f, 1.0f}});
}

void GameLayer::onDetach() {}

void GameLayer::onUpdate(vektor::core::Timestep ts)
{
    if (m_GameOver)
        return;

    m_PlayerVelocity.x = 0.0f;
    if (vektor::input::Input::isKeyPressed(vektor::KeyCode::A))
        m_PlayerVelocity.x = -m_PlayerSpeed;
    else if (vektor::input::Input::isKeyPressed(vektor::KeyCode::D))
        m_PlayerVelocity.x = m_PlayerSpeed;

    if (vektor::input::Input::isKeyPressed(vektor::KeyCode::Space) && m_CanJump)
    {
        m_PlayerVelocity.y = m_JumpForce;
        m_CanJump = false;
    }

    m_PlayerVelocity.y += m_Gravity * ts;

    m_Player.Position.x += m_PlayerVelocity.x * ts;
    for (auto &obj : m_LevelData)
    {
        if (checkCollision(m_Player, obj))
        {
            m_Player.Position.x -= m_PlayerVelocity.x * ts;
        }
    }

    m_Player.Position.y += m_PlayerVelocity.y * ts;
    m_CanJump = false;  
    for (auto &obj : m_LevelData)
    {
        if (checkCollision(m_Player, obj))
        {
            if (m_PlayerVelocity.y < 0)
            { // Falling
                m_CanJump = true;
                m_Player.Position.y = obj.Position.y + (obj.Size.y / 2.0f) + (m_Player.Size.y / 2.0f);
            }
            else
            {  
                m_Player.Position.y = obj.Position.y - (obj.Size.y / 2.0f) - (m_Player.Size.y / 2.0f);
            }
            m_PlayerVelocity.y = 0;
        }
    }

    if (m_Player.Position.y < -10.0f)
    {
        m_GameOver = true;
    }

    glm::vec3 camPos = m_CameraController->getCameraPosition();
    camPos.x += (m_Player.Position.x - camPos.x) * ts * 2.0f;
    camPos.y += (m_Player.Position.y - camPos.y) * ts * 2.0f;
    m_CameraController->setCameraPosition(camPos);

    m_CameraController->onUpdate(ts);

    vektor::renderer::Command::setClearColor({0.1f, 0.1f, 0.15f, 1.0f});
    vektor::renderer::Command::clear();

    vektor::renderer::Renderer2D::beginScene(m_CameraController->getCamera());

    for (auto &obj : m_LevelData)
        vektor::renderer::Renderer2D::drawQuad(obj.Position, obj.Size, obj.Color);

    vektor::renderer::Renderer2D::drawQuad(m_Player.Position, m_Player.Size, m_Player.Color);

    vektor::renderer::Renderer2D::endScene();
}

void GameLayer::onRender()
{
    ImGui::Begin("Game Stats");
    ImGui::Text("Player Pos: %.2f, %.2f", m_Player.Position.x, m_Player.Position.y);
    ImGui::Text("Velocity Y: %.2f", m_PlayerVelocity.y);
    ImGui::Text("On Ground: %s", m_CanJump ? "Yes" : "No");

    if (m_GameOver)
    {
        ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, "GAME OVER!");
        if (ImGui::Button("Restart"))
            resetGame();
    }

    ImGui::End();
}

void GameLayer::onEvent(vektor::event::Event &event)
{
    m_CameraController->onEvent(event);
}

bool GameLayer::checkCollision(const GameObject &a, const GameObject &b)
{
    bool collisionX = a.Position.x + a.Size.x / 2 >= b.Position.x - b.Size.x / 2 &&
                      b.Position.x + b.Size.x / 2 >= a.Position.x - a.Size.x / 2;

    bool collisionY = a.Position.y + a.Size.y / 2 >= b.Position.y - b.Size.y / 2 &&
                      b.Position.y + b.Size.y / 2 >= a.Position.y - a.Size.y / 2;

    return collisionX && collisionY;
}