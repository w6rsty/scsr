#include "core/object.hpp"
#include "core/event/handler.hpp"

namespace scsr
{

World::World() :
    running(false),
    eventHandler(*this)
{
    RegisterEvent<AppExitEvent>([this](Event, Storage& storage) {
        running = false;
    });
}

bool World::ShouldExit()
{
    return running;
}

void World::Run()
{
    running = true;
    while (ShouldExit())
    {
        // Poll events

        eventHandler.Poll();
        eventHandler.Dispatch();

        for (auto& system : storage.systems)
        {
            system(storage, eventHandler);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

}