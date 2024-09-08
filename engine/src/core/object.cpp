#include "core/object.hpp"
#include "core/io.hpp"

namespace scsr
{

World::World() :
    running(false),
    eventHandler(*this)
{
    RegisterEvent<AppExitEvent>([this](Event, Storage& storage) {
        running = false;
    });

    RegisterObject<Ticker>();
}

bool World::ShouldExit()
{
    return running;
}

void World::Run()
{
    for (auto& startup : storage.startups)
    {
        startup(*this, storage);
    }

    running = true;
    while (ShouldExit())
    {
        auto start = std::chrono::high_resolution_clock::now();

        {
            PROFILE_SCOPE("Event handle");
            eventHandler.Poll();
            eventHandler.Dispatch();
        }

        {
            PROFILE_SCOPE("System update");
            for (auto& system : storage.systems)
            {
                system(storage, eventHandler);
            }
        }

        std::chrono::duration<f64> elapsed = std::chrono::high_resolution_clock::now() - start;
        storage.GetObject<Ticker>().delta = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        ++(storage.GetObject<Ticker>().tick);
    }
}

}