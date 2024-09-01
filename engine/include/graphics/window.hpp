#pragma once

#include "core/event/event.hpp"
#include "core/type.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace scsr
{

struct WindowProp
{
    std::string Title;
    u32 Width;
    u32 Height;
};

class Window
{
    SIG(Window)
public:
    Window(WindowProp prop);
    ~Window();

    Window(Window&& other);
    Window& operator=(Window&& other);

    void OnUpdate();
    void Draw();
private:
    bool m_Status = false;
    SDL_Window* m_Handle;
    SDL_Renderer* m_Renderer;
    SDL_Texture* m_Texture;

    WindowProp m_Prop;
};

}