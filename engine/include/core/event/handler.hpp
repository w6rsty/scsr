#pragma once

#include "event.hpp"
#include "core/ds/msqueue.hpp"

namespace scsr
{

class EventHandler
{
public:
    void PushEvent(const Event& event);
    void Dispatch();
private:
    MSQueue<Event> m_EventQueue;
};

}