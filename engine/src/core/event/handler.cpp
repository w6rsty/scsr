#include "core/event/handler.hpp"
#include "core/event/event.hpp"
#include "core/object.hpp"

#include "SDL_events.h"

namespace scsr
{

EventHandler::EventHandler(World& world) :
    m_Filter(EventCategory::ECALL),
    m_World(world)
{}

void EventHandler::Poll()
{
    SDL_Event sdlEvent;
    Event event;
    while (SDL_PollEvent(&sdlEvent))
    {
        auto& type = sdlEvent.type;

        switch (type)
        {
            case SDL_QUIT:
                event.applicationQuit = AppExitEvent {};
                break;
            case SDL_WINDOWEVENT:
            {
                switch (sdlEvent.window.event)
                {
                    case SDL_WINDOWEVENT_SHOWN:
                        event.windowShow = WindowShowEvent { .windowID = sdlEvent.window.windowID };
                        break;
                    case SDL_WINDOWEVENT_MOVED:
                        event.windowMove = WindowMoveEvent { .windowID = sdlEvent.window.windowID, .x = sdlEvent.window.data1, .y = sdlEvent.window.data2 };
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        event.windowResize = WindowResizeEvent { .windowID = sdlEvent.window.windowID, .x = sdlEvent.window.data1, .y = sdlEvent.window.data2 };
                        break;
                    case SDL_WINDOWEVENT_MINIMIZED:
                        event.windowMinimize = WindowMinimizeEvent { .windowID = sdlEvent.window.windowID };
                        break;
                    case SDL_WINDOWEVENT_MAXIMIZED:
                        event.windowMaximize = WindowMaximizeEvent { .windowID = sdlEvent.window.windowID };
                        break;
                    case SDL_WINDOWEVENT_ENTER:
                        event.windowEnter = WindowEnterEvent { .windowID = sdlEvent.window.windowID };
                        break;
                    case SDL_WINDOWEVENT_LEAVE:
                        event.windowLeave = WindowLeaveEvent { .windowID = sdlEvent.window.windowID };
                        break;
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        event.windowFocusGained = WindowFocusGainedEvent { .windowID = sdlEvent.window.windowID };
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        event.windowFocusLost = WindowFocusLostEvent { .windowID = sdlEvent.window.windowID };
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        event.windowClose = WindowCloseEvent { .windowID = sdlEvent.window.windowID };
                        break;
                    default:
                        continue;
                }
                break;
            }
            case SDL_KEYDOWN:
                event.keyboardPressed = KeyboardPressedEvent { .keyCode = (i32)sdlEvent.key.keysym.sym, .repeat = sdlEvent.key.repeat };
                break;
            case SDL_KEYUP:
                event.keyboardReleased = KeyboardReleasedEvent { .keyCode = (i32)sdlEvent.key.keysym.sym, .repeat = sdlEvent.key.repeat };
                break;
            case SDL_MOUSEMOTION:
                event.mouseMotion = MouseMotionEvent { .x = sdlEvent.motion.x, .y = sdlEvent.motion.y, .dx = sdlEvent.motion.xrel, .dy = sdlEvent.motion.yrel };
                break;
            case SDL_MOUSEBUTTONDOWN:
                event.mouseButtonPressed = MouseButtonPressedEvent { .button = sdlEvent.button.button, .clicks = sdlEvent.button.clicks, .cx = sdlEvent.button.x, .cy = sdlEvent.button.y };
                break;
            case SDL_MOUSEBUTTONUP:
                event.mouseButtonReleased = MouseButtonReleasedEvent { .button = sdlEvent.button.button, .clicks = sdlEvent.button.clicks, .cx = sdlEvent.button.x, .cy = sdlEvent.button.y };
                break;
            case SDL_MOUSEWHEEL:
                event.mouseWheel = MouseWheelEvent { .x = sdlEvent.wheel.x, .y = sdlEvent.wheel.y, .cx = sdlEvent.wheel.mouseX, .cy = sdlEvent.wheel.mouseY };
                break;
            default:
                continue;
        }


        m_EventQueue.Enqueue(event);
    }
}

void EventHandler::Dispatch()
{
    Event event;
    while (m_EventQueue.Dequeue(event))
    {
        if (event.category & m_Filter)
        {
            auto it = m_Callback.find(event.type);
            if (it != m_Callback.end())
            {
                for (auto& callback : it->second)
                {
                    callback(event, m_World.storage);
                }
            }
        }
    } 
}

void EventHandler::SetFilter(u16 filter)
{
    m_Filter = filter;
}

void EventHandler::SetCallback(EventType type, std::function<void(Event, Storage&)> callback)
{
    m_Callback[type].emplace_back(callback);
}

bool IsKeyUp(KeyboardKeyCode code)
{
    // TODO: Implement
    return false;
}

bool IsKeyDown(KeyboardKeyCode code)
{
    // TODO: Implement
    return false;
}

}


