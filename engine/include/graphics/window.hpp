#pragma once

#include "core/type.hpp"

#include <string>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace scsr
{

class Image;

struct WindowProp
{
    std::string title;
    i32 width;
    i32 height;
};

class Window
{   
    SIG(Window)
public:
    Window(WindowProp prop);
    ~Window();

    void OnUpdate(Ref<Image> image);
private:
    bool m_Status = false;
    void* m_NativeHandle;
    WindowProp m_Prop;
};

}