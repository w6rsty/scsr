#pragma once

#include "core/format.hpp"
#include "event.hpp"
#include "core/ds/msqueue.hpp"

#include <functional>
#include <unordered_map>

namespace scsr
{

struct World;
struct Storage;

class EventHandler
{
    PIN(EventHandler)
    SIG(EventHandler)
public:
    EventHandler(World& world);

    void Poll();
    void Dispatch();
    void SetFilter(u16 filter);
    void SetCallback(EventType type, std::function<void(Event, Storage&)> callback);
private:
    u16 m_Filter;
    MSQueue<Event> m_EventQueue;
    std::unordered_map<EventType, std::vector<std::function<void(Event, Storage&)>>> m_Callback;
    World& m_World;
};

bool IsKeyUp(KeyboardKeyCode code);
bool IsKeyDown(KeyboardKeyCode code);

}