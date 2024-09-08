#include "graphics/window.hpp"
#include "core/log.hpp"
#include "core/io.hpp"

#include "SDL.h"

namespace scsr
{ 

Window::Window(const std::string& title, i32 width, i32 height) :
    m_Title(title),
    m_Width(width),
    m_Height(height)
{
    m_Context = MakeScp<Context>();
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    m_Context->handle = SDL_CreateWindow(
        m_Title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_Width, m_Height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    m_Status = m_Context->handle != nullptr;
    LOG_INFO("SDL context created");

    m_Context->renderer = SDL_CreateRenderer(
        m_Context->handle,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    m_Context->texture = SDL_CreateTexture(
        m_Context->renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,    
        800, 600
    );
}

Window::~Window()
{
    if (m_Status)
    {
        SDL_DestroyTexture(m_Context->texture);
        SDL_DestroyRenderer(m_Context->renderer);
        SDL_DestroyWindow(m_Context->handle);

        SDL_Quit();
        LOG_INFO("SDL context destroyed");
    }
}

void Window::OnUpdate(std::function<void(void*, usize)> fn)
{
    
    void* ptr;
    int pitch;

    {
        PROFILE_SCOPE("Lock & unlock");
        SDL_LockTexture(m_Context->texture, nullptr, &ptr, &pitch);
        memset(ptr, 0, pitch * m_Height);
        fn(ptr, pitch);
        SDL_UnlockTexture(m_Context->texture);
    }

    int w, h;
    SDL_GetWindowSize(m_Context->handle, &w, &h);
    SDL_Rect rect { .x = 0, .y = 0, .w = w, .h = h };

    {
        PROFILE_SCOPE("Texture copy & present");
        SDL_RenderClear(m_Context->renderer);
        SDL_RenderCopy(m_Context->renderer, m_Context->texture, nullptr, &rect);
        SDL_RenderPresent(m_Context->renderer);
    }
}

}