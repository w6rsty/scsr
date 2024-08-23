#include "graphics/window.hpp"
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
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return false;
        }
    }

    SDL_RenderClear(m_Renderer);
    SDL_RenderCopy(m_Renderer, m_Texture, nullptr, nullptr);
    SDL_RenderPresent(m_Renderer);

    return true;
}

}