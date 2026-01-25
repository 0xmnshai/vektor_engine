
#include "vektor.hpp"
#include "entry_point.hpp"

class Sandbox : public vektor::Application
{
public:
    Sandbox()
    {
    }

    ~Sandbox()
    {
    }
};

vektor::Application *vektor::CreateApplication()
{
    return new Sandbox();
}