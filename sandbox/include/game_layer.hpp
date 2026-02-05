#pragma once

#include <vector>

#include "vektor.hpp"

struct GameObject
{
    glm::vec2 Position;
    glm::vec2 Size;
    glm::vec4 Color;
    bool Collidable = true;
};

class GameLayer : public vektor::layer::Layer
{
public:
    GameLayer();
    virtual ~GameLayer() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;
    virtual void onUpdate(vektor::core::Timestep timestep) override;
    virtual void onRender() override;
    virtual void onEvent(vektor::event::Event &event) override;

private:
    bool checkCollision(const GameObject &a, const GameObject &b);
    void resetGame();

private:
    std::shared_ptr<vektor::renderer::camera::Controller> m_CameraController;

    GameObject m_Player;
    glm::vec2 m_PlayerVelocity = {0.0f, 0.0f};
    bool m_CanJump = false;

    const float m_Gravity = -9.81f;
    const float m_JumpForce = 8.0f;
    const float m_PlayerSpeed = 5.0f;

    std::vector<GameObject> m_LevelData;

    uint32_t m_Score = 0;
    bool m_GameOver = false;
};