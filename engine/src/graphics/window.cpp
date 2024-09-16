#include "graphics/window.hpp"
#include "graphics/image.hpp"
#include "core/log.hpp"

#include <SDL.h>
#include <Tracy.hpp>

namespace scsr
{ 

Window::Window(WindowProp prop) :
    m_Prop(prop)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    m_NativeHandle = SDL_CreateWindow(
        prop.title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_Prop.width, m_Prop.height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    m_Status = m_NativeHandle != nullptr;
    LOG_INFO("SDL context created");
}

Window::~Window()
{
    if (m_Status)
    {
        SDL_DestroyWindow(static_cast<SDL_Window*>(m_NativeHandle));

        SDL_Quit();
        LOG_INFO("SDL context destroyed");
    }
}

void Window::OnUpdate(Ref<Image> image)
{
    ZoneScopedN("Window::OnUpdate");
    SDL_Window* window = static_cast<SDL_Window*>(m_NativeHandle);
    i32 w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect rect { .w = w, .h = h };
    
    {
        ZoneScopedN("Image Blit");
        SDL_FillRect(
            SDL_GetWindowSurface(window),
            &rect, 
            SDL_MapRGB(SDL_GetWindowSurface(window)->format, 0, 0, 0)
        );
        FrameImage(image->Data(), image->Width(), image->Height(), 0, false);
        SDL_BlitScaled(
            static_cast<SDL_Surface*>(image->SurfaceHandle()),
            nullptr, 
            SDL_GetWindowSurface(window),
            &rect
        );
    }
    {
        ZoneScopedN("SDL Update");
        SDL_UpdateWindowSurface(window);
    }
}

}