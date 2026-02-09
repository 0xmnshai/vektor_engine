
#include "entry_point.hpp"

// samples
// #include "2d_layer.hpp"
// #include "game_layer.hpp"
// #include "example_layer.hpp"
// #include "particle_layer.hpp"
#include "editor_layer.hpp"

class Sandbox : public vektor::Application
{
public:
    Sandbox()
        : Application("Sandbox")
    {
        // pushLayer(new ExampleLayer());
        // pushLayer(new GameLayer());
        // pushLayer(new ParticleLayer());
        // pushLayer(new VektorEditor2D());
        pushLayer(new EditorLayer());
    }

    ~Sandbox()
    {
    }
};

vektor::Application *vektor::createApplication()
{
    return new Sandbox();
}