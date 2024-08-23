#include "graphics/window.hpp"
#include "SDL_video.h"
#include "core/event/event.hpp"
#include "core/log.hpp"
#include "core/type.hpp"

#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "stb_image.h"

namespace scsr
{

const u32 WIDTH = 800;
const u32 HEIGHT = 600;

Window::Window()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    m_Handle = SDL_CreateWindow(
        "scsr",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    LOG_INFO("SDL context created");

    m_Renderer = SDL_CreateRenderer(
        m_Handle,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    m_Texture = SDL_CreateTexture(
        m_Renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    // unsigned char* image = (unsigned char*)m_Texture; 
    // SDL_UpdateTexture(m_Texture, nullptr, image, 1280 * sizeof(u32));
}

Window::~Window()
{
    SDL_DestroyTexture(m_Texture);
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Handle);

    SDL_Quit();

}

bool Window::OnUpdate()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        Event event;

        switch (sdlEvent.type)
        {
            // Application events
            case SDL_QUIT:
            {
                event.application = ApplicationEvent
                {
                    .signal = ApplicationSignal::Quit,
                };
                m_EventHandler.PushEvent(event);
                return false;
            }
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            {
                event.application = ApplicationEvent
                {
                    .signal = ApplicationSignal::FocusGained,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            case SDL_WINDOWEVENT_FOCUS_LOST:
            {
                event.application = ApplicationEvent
                {
                    .signal = ApplicationSignal::FocusLost,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            case SDL_WINDOWEVENT_HIDDEN:
            {
                event.application = ApplicationEvent
                {
                    .signal = ApplicationSignal::Hidden,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            case SDL_WINDOWEVENT_ENTER:
            {
                event.application = ApplicationEvent
                {
                    .signal = ApplicationSignal::Enter,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            case SDL_WINDOWEVENT_LEAVE:
            {
                event.application = ApplicationEvent
                {
                    .signal = ApplicationSignal::Leave,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            // Window events
            case SDL_WINDOWEVENT_CLOSE:
            {
                event.window = WindowEvent
                {
                    .signal = WindowSignal::Close,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            case SDL_WINDOWEVENT_RESIZED:
            {
                event.window = WindowEvent
                {
                    .signal = WindowSignal::Resize,
                    .x = sdlEvent.window.data1,
                    .y = sdlEvent.window.data2,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            case SDL_WINDOWEVENT_MOVED:
            {
                event.window = WindowEvent
                {
                    .signal = WindowSignal::Move,
                    .x = sdlEvent.window.data1,
                    .y = sdlEvent.window.data2,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            // Keyboard events
            case SDL_KEYDOWN:
            {
                event.keyboard = KeyboardEvent
                {
                    .keyCode = (u32)sdlEvent.key.keysym.sym,
                    .state = ButtonState::Pressed,
                    .repeat = (u32)sdlEvent.key.repeat,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            case SDL_KEYUP:
            {
                event.keyboard = KeyboardEvent
                {
                    .keyCode = (u32)sdlEvent.key.keysym.sym,
                    .state = ButtonState::Released,
                    .repeat = 0,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            // Mouse motion events
            case SDL_MOUSEMOTION:
            {
                event.mouseMotion = MouseMotionEvent
                {
                    .x = sdlEvent.motion.x,
                    .y = sdlEvent.motion.y,
                    .dx = sdlEvent.motion.xrel,
                    .dy = sdlEvent.motion.yrel,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            // Mouse button events
            case SDL_MOUSEBUTTONDOWN:
            {
                event.mouseButton = MouseButtonEvent
                {
                    .button = (MouseButton)sdlEvent.button.button,
                    .state = ButtonState::Pressed,
                    .clicks = (u32)sdlEvent.button.clicks,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                event.mouseButton = MouseButtonEvent
                {
                    .button = (MouseButton)sdlEvent.button.button,
                    .state = ButtonState::Released,
                    .clicks = (u32)sdlEvent.button.clicks,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            // Mouse wheel events
            case SDL_MOUSEWHEEL:
            {
                event.mouseWheel = MouseWheelEvent
                {
                    .offset = sdlEvent.wheel.y,
                };
                m_EventHandler.PushEvent(event);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    m_EventHandler.Dispatch();

    SDL_RenderClear(m_Renderer);
    SDL_RenderCopy(m_Renderer, m_Texture, nullptr, nullptr);
    SDL_RenderPresent(m_Renderer);

    return true;
}

}