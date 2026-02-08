
#include "entry_point.hpp"

// samples
#include "vektor_editor_2D.hpp"
#include "game_layer.hpp"
#include "example_layer.hpp"
#include "particle_layer.hpp"

class Sandbox : public vektor::Application
{
public:
    Sandbox()
        : Application("Sandbox")
    {
        // pushLayer(new ExampleLayer());
        // pushLayer(new Sandbox2D());
        // pushLayer(new GameLayer());
        // pushLayer(new ParticleLayer());
    }

    ~Sandbox()
    {
    }
};

vektor::Application *vektor::createApplication()
{
    return new Sandbox();
}