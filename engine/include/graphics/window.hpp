#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace scsr
{

class Window
{
public:
    Window();
    ~Window();

    bool OnUpdate();
private:
    SDL_Window* m_Handle;
    SDL_Renderer* m_Renderer;
    SDL_Texture* m_Texture;
};

}