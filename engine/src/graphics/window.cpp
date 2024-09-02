#include "graphics/window.hpp"
#include "core/log.hpp"

#include "SDL.h"

namespace scsr
{ 

Window::Window(WindowProp prop) :
    m_Prop(prop)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    m_Handle = SDL_CreateWindow(
        m_Prop.Title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_Prop.Width, m_Prop.Height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    m_Status = m_Handle != nullptr;
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
        m_Prop.Width, m_Prop.Height
    );
}

Window::~Window()
{
    if (m_Status)
    {
        SDL_DestroyTexture(m_Texture);
        SDL_DestroyRenderer(m_Renderer);
        SDL_DestroyWindow(m_Handle);

        SDL_Quit();
        LOG_INFO("SDL context destroyed");
    }
}

Window::Window(Window&& other)
{
    m_Status = other.m_Status;
    m_Handle = other.m_Handle;
    m_Renderer = other.m_Renderer;
    m_Texture = other.m_Texture;
    m_Prop = other.m_Prop;

    other.m_Status = false;
    other.m_Handle = nullptr;
    other.m_Renderer = nullptr;
    other.m_Texture = nullptr;
}

Window& Window::operator=(Window&& other)
{
    if (this != &other)
    {
        m_Status = other.m_Status;
        m_Handle = other.m_Handle;
        m_Renderer = other.m_Renderer;
        m_Texture = other.m_Texture;
        m_Prop = other.m_Prop;

        other.m_Status = false;
        other.m_Handle = nullptr;
        other.m_Renderer = nullptr;
        other.m_Texture = nullptr;
    }

    return *this;
}

void Window::OnUpdate(std::function<void(void*, usize)> fn)
{
    void* ptr;
    int pitch;
    SDL_LockTexture(m_Texture, nullptr, &ptr, &pitch);
    fn(ptr, pitch);
    SDL_UnlockTexture(m_Texture);


    SDL_Rect rect { .x = 0, .y = 0, .w = (int)m_Prop.Width, .h = (int)m_Prop.Height };

    SDL_RenderCopy(m_Renderer, m_Texture, nullptr, &rect);
    SDL_RenderPresent(m_Renderer);
}

}