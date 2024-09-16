#include "graphics/image.hpp"
#include "core/math/math.hpp"

#include <SDL.h>
#include <Tracy.hpp>

namespace scsr
{

Image::Image(ImageProp prop) :
    m_Prop(prop)
{
    Create();
}

Image::~Image()
{
    Release();
}

void Image::Clear()
{
    ZoneScopedN("Image Clear");
    memset(m_Data, 0, m_Prop.width * m_Prop.height * 4);
}

void Image::Resize(ImageProp prop)
{
    Release();
    m_Prop = prop;
    Create();
}

void Image::SetPixel(i32 x, i32 y, u32 color)
{
    usize index = y * m_Prop.width + x;
    m_Data[index] = color;
}

void Image::SetPixel(Vec2i p, u32 color)
{
    SetPixel(p.x, p.y, color);
}

void Image::SetLine(i32 x0, i32 y0, i32 x1, i32 y1, u32 color)
{
    ZoneScoped;
    i32 dx = Abs(x1 - x0);
    i32 dy = Abs(y1 - y0);
    i32 sx = x0 < x1 ? 1 : -1;
    i32 sy = y0 < y1 ? 1 : -1;
    i32 err = dx - dy;

    while (true)
    {
        SetPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) { break; }
        i32 e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void Image::SetLine(Vec2i p0, Vec2i p1, u32 color)
{
    SetLine(p0.x, p0.y, p1.x, p1.y, color);
}

i32 Image::Width() const { return m_Prop.width; }

i32 Image::Height() const { return m_Prop.height; }

u32* Image::Data() { return m_Data; }

void* Image::SurfaceHandle() const { return m_Surface; }

void Image::Create()
{
    m_Data = new u32[m_Prop.width * m_Prop.height];
    m_Surface = SDL_CreateRGBSurfaceFrom(
        m_Data,
        m_Prop.width, m_Prop.height, 32, m_Prop.width * 4,
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
    );
}

void Image::Release()
{
    if (m_Surface)
    {
        SDL_FreeSurface(static_cast<SDL_Surface*>(m_Surface));
        m_Surface = nullptr;
    }
    if (m_Data)
    {
        delete [] m_Data;
        m_Data = nullptr;
    }
}

}