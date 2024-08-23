#include "core/event/handler.hpp"
#include "core/event/event.hpp"
#include "core/log.hpp"

#include "core/event/format.hpp"

namespace scsr
{

void EventHandler::PushEvent(const Event& event)
{
    m_EventQueue.Enqueue(event);
}

void EventHandler::Dispatch()
{
   while (auto event = m_EventQueue.Dequeue())
   {
        if (event.has_value())
        {
            LOG_INFO("{}", EventFormat(event.value()));
        }
   } 
}

}