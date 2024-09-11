#include "camera.hpp"
#include "render.hpp"

using namespace scsr;

int runtime(int argc, char* argv[])
{
    WindowProp prop { .title = "scsr", .width = 800, .height = 600 };

    World()
        .RegisterObject<Window>(prop)
        .AddPlugin(CameraPlugin)
        .AddPlugin(RenderPlugin)
        .Run();

    return 0;
}