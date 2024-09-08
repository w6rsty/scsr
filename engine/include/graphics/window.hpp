#pragma once

#include "core/type.hpp"

#include <string>
#include <functional>

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

struct Context
{
    SDL_Window* handle;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

class Window
{   
    SIG(Window)
public:
    Window(const std::string& title, i32 width, i32 height);
    ~Window();

    /// Temporary hack
    void OnUpdate(std::function<void(void*, usize)> fn);
private:
    bool m_Status = false;
    Scp<Context> m_Context;

    std::string m_Title;
    i32 m_Width;
    i32 m_Height;
};

}