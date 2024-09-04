#include "core/core.hpp"

using namespace scsr;

void Update(Storage& storage)
{
    auto& window = storage.GetObject<Window>();
    auto& pipeline = storage.GetObject<Pipeline>();

    window.OnUpdate([&](void* ptr, usize pitch) {
        pipeline.Perform(ptr, pitch);
    });
}

int runtime(int argc, char* argv[])
{
    WindowProp prop { .Title = "scsr", .Width = 600, .Height = 600 };

    World()
        .RegisterObject<Window>(prop)
        .RegisterObject<Pipeline>()
        .AddSystem(Update)
        .RegisterEvent<MouseButtonPressedEvent>([&](Event event, Storage& storage) {
            LOG_INFO("Summon an demon");
        })
        .Run();

    return 0;
}