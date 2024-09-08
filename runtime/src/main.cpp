#include "camera.hpp"
#include "render.hpp"

using namespace scsr;

void Update(Storage& storage)
{
    auto& window = storage.GetObject<Window>();
    auto& pipeline = storage.GetObject<Pipeline>();
    
    {
        PROFILE_SCOPE("Window update");
        window.OnUpdate([&](void* ptr, usize pitch) {
            pipeline.Perform(ptr, pitch);
        });
    }
}

int runtime(int argc, char* argv[])
{
    World()
        .RegisterObject<Window>("scsr", 800, 600)
        .AddStartup(CameraPlugin)
        .AddStartup(RenderPlugin)
        .AddSystem(Update)
        .Run();

    return 0;
}