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
        m_Prop.Width / 20, m_Prop.Height / 20
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

void Window::OnUpdate()
{
    Draw();
    SDL_Rect rect { .x = 0, .y = 0, .w = (int)m_Prop.Width, .h = (int)m_Prop.Height };

    SDL_RenderCopy(m_Renderer, m_Texture, nullptr, &rect);
    SDL_RenderPresent(m_Renderer);
}

void Window::Draw()
{

    void* ptr;
    int pitch;
    SDL_LockTexture(m_Texture, nullptr, &ptr, &pitch);

    u32 w = m_Prop.Width / 20;
    u32 h = m_Prop.Height / 20;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x <w ; ++x) {
            u32* pixel = (u32*)((u8*)ptr + y * pitch + x * 4);
            float r = (float)x / (w - 1);
            float g = (float)y / (h - 1);
            float b = 0;
            *pixel = (u32)(255) << 24 | (u32)(b * 255) << 16 | (u32)(g * 255) << 8 | (u32)(r * 255);
        }
    }

    SDL_UnlockTexture(m_Texture);
}

}