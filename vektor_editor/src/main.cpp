
#include "entry_point.hpp"

#include "editor_layer.hpp"

class VektorEditor : public vektor::Application
{
public:
    VektorEditor()
        : Application("Vektor Editor")
    {
        pushLayer(new EditorLayer());
    }

    ~VektorEditor()
    {
    }
};

vektor::Application *vektor::createApplication()
{
    return new VektorEditor();
}