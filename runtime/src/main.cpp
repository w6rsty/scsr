#include "core/core.hpp"

using namespace scsr;

void Update(Storage& storage)
{
    auto& window = storage.Get<Window>();
    auto& pipeline = storage.Get<Pipeline>();

    window.OnUpdate([&pipeline](void* ptr, usize pitch) {
        pipeline.Perform(ptr, pitch);
    });
}

int runtime(int argc, char* argv[])
{
    WindowProp prop { .Title = "scsr", .Width = 600, .Height = 600 };

    World()
        .InitObject(Window {prop})
        .InitObject(Pipeline {})
        .AddSystem(Update)
        .Run();

    return 0;
}