#pragma once

#include "event.hpp"
#include "core/ds/msqueue.hpp"

namespace scsr
{

struct World;

class EventHandler
{
    PIN(EventHandler)
    SIG(EventHandler)
public:
    EventHandler(World& world);

    void Poll();
    void Dispatch();
    void SetFilter(u16 filter);
private:
    MSQueue<Event> m_EventQueue;
    u16 m_Filter;
    World& m_World;
};

}