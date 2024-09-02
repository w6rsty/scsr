#include "core/object.hpp"
#include "core/event/handler.hpp"

namespace scsr
{

ObjectIndex ObjectIndexer::s_Counter = 0;

World::World() :
    eventHandler(*this)
{

}

void World::Run()
{
    while (true)
    {
        // Poll events

        eventHandler.Poll();
        eventHandler.Dispatch();

        for (auto& system : storage.systems) {
            system(storage, eventHandler);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

}