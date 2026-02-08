
#include "entry_point.hpp"

#include "sandbox.hpp"

class SandboxApp : public vektor::Application
{
public:
    SandboxApp()
        : Application("Sandbox")
    {
        pushLayer(new Sandbox());
    }

    ~SandboxApp()
    {
    }
};

vektor::Application *vektor::createApplication()
{
    return new SandboxApp();
}