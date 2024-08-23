#include "core/event/handler.hpp"
#include "core/event/event.hpp"
#include "core/log.hpp"

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
    auto category = event->GetCategory();
    auto type = event->GetType();

    LOG_INFO(
        "Event [{}: {}]",
        EventCategoryToString(category),
        EventTypeToString(type)
    );
   } 
}

}