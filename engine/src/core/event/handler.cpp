#include "core/event/handler.hpp"
#include "core/log.hpp"
#include "core/event/format.hpp"

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
        if (type == SDL_QUIT)
        {
            event.application = ApplicationEvent {
                .signal = ApplicationSignal::Quit,
            };
        }
        else if (type == SDL_WINDOWEVENT)
        {
            event.category = EventCategory::ECWindow;
            event.window.windowID = sdlEvent.window.windowID;
            auto& windowEvent = sdlEvent.window.event;
            if (windowEvent == SDL_WINDOWEVENT_SHOWN)
            {
                event.window.signal = WindowSignal::Shown;
            }
            else if (windowEvent == SDL_WINDOWEVENT_HIDDEN)
            {
                event.window.signal = WindowSignal::Hidden;
            }
            else if (windowEvent == SDL_WINDOWEVENT_MOVED)
            {
                event.window.signal = WindowSignal::Moved;
                event.window.x = sdlEvent.window.data1;
                event.window.y = sdlEvent.window.data2;
            }
            else if (windowEvent == SDL_WINDOWEVENT_RESIZED)
            {
                event.window.signal = WindowSignal::Resized;
                event.window.x = sdlEvent.window.data1;
                event.window.y = sdlEvent.window.data2;
            }
            else if (windowEvent == SDL_WINDOWEVENT_MINIMIZED)
            {
                event.window.signal = WindowSignal::Minimized;
            }
            else if (windowEvent == SDL_WINDOWEVENT_MAXIMIZED)
            {
                event.window.signal = WindowSignal::Maximized;
            }
            else if (windowEvent == SDL_WINDOWEVENT_ENTER)
            {
                event.window.signal = WindowSignal::Enter;
            }
            else if (windowEvent == SDL_WINDOWEVENT_LEAVE)
            {
                event.window.signal = WindowSignal::Leave;
            }
            else if (windowEvent == SDL_WINDOWEVENT_FOCUS_GAINED)
            {
                event.window.signal = WindowSignal::FocusGained;
            }
            else if (windowEvent == SDL_WINDOWEVENT_FOCUS_LOST)
            {
                event.window.signal = WindowSignal::FocusLost;
            }
            else if (windowEvent == SDL_WINDOWEVENT_CLOSE)
            {
                event.window.signal = WindowSignal::Close;
            }
            else
            {
                continue;
            }
        }
        else if (type == SDL_KEYDOWN)
        {
            event.keyboard = KeyboardEvent {
                .keyCode = (u32)sdlEvent.key.keysym.sym,
                .state = ButtonState::Pressed,
                .repeat = (u32)sdlEvent.key.repeat,
            };
        }
        else if (type == SDL_KEYUP)
        {
            event.keyboard = KeyboardEvent {
                .keyCode = (u32)sdlEvent.key.keysym.sym,
                .state = ButtonState::Released,
                .repeat = 0,
            };
        }
        else if (type == SDL_MOUSEMOTION)
        {
            event.mouseMotion = MouseMotionEvent {
                .x = sdlEvent.motion.x,
                .y = sdlEvent.motion.y,
                .dx = sdlEvent.motion.xrel,
                .dy = sdlEvent.motion.yrel,
            };
        }
        else if (type == SDL_MOUSEBUTTONDOWN)
        {
            event.mouseButton = MouseButtonEvent {
                .button = (MouseButton)sdlEvent.button.button,
                .state = ButtonState::Pressed,
                .clicks = (u32)sdlEvent.button.clicks,
            };
        }
        else if (type == SDL_MOUSEBUTTONUP)
        {
            event.mouseButton = MouseButtonEvent {
                .button = (MouseButton)sdlEvent.button.button,
                .state = ButtonState::Released,
                .clicks = (u32)sdlEvent.button.clicks,
            };
        }
        else if (type == SDL_MOUSEWHEEL)
        {
            event.mouseWheel = MouseWheelEvent {
                .x = sdlEvent.wheel.x,
                .y = sdlEvent.wheel.y,
            };
        }
        else
        {
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
            LOG_INFO("{}", FormatEvent(event));
        }
    } 
}

void EventHandler::SetFilter(u16 filter)
{
    m_Filter = filter;
}


}


