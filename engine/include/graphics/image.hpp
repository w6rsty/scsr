#pragma once

#include "core/type.hpp"
#include "core/math/math.hpp"

namespace scsr
{

struct ImageProp
{
    i32 width;
    i32 height;
};

class Image
{
public:
    Image(ImageProp prop);
    ~Image();

    void Clear();
    void Resize(ImageProp prop);

    void SetPixel(i32 x, i32 y, u32 color);
    void SetPixel(Vec2i p, u32 color);
    void SetLine(i32 x0, i32 y0, i32 x1, i32 y1, u32 color);
    void SetLine(Vec2i p0, Vec2i p1, u32 color);


    void SetDepth(Vec2i p, f32 depth);
    f32 ReadDepth(Vec2i p) const;

    bool TestDepth(Vec2i p, f32 depth);

    void TestDepthAndSetPixel(Vec2i p, f32 depth, u32 color);

    void ClearDepth();

    u32* Data() { return m_Data; }
    i32 Width() const { return m_Prop.width; }
    i32 Height() const { return m_Prop.height; }
    void* SurfaceHandle() const { return m_Surface; }
private:
    void Create();
    void Release();

    u32* m_Data;
    f32* m_DepthBuffer;
    void* m_Surface;
    ImageProp m_Prop;
};

}