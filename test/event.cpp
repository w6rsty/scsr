#include "core/event/handler.hpp"

using namespace scsr;

int main() {
    EventHandler handler;

    Event a = Event(EventType::WindowClose, 0, 0);
    Event b = Event(EventType::KeyPressed, 0, 0);
    Event c = Event(EventType::MouseMoved, 0, 0);

    handler.PushEvent(a);
    handler.PushEvent(b);
    handler.PushEvent(c);

    handler.Dispatch();
    return 0;
}