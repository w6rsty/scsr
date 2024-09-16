#include "graphics/vertex.hpp"
#include "core/math/vector.hpp"

#include <Tracy.hpp>

namespace scsr
{

Vertex FromInterpolation(const Vertex& v1, const Vertex& v2, f32 t)
{
    ZoneScopedN("Vertex interpolation");

    Vertex vtx;
    vtx.pos = Lerp(v1.pos, v2.pos, t);
    vtx.uv = Lerp(v1.uv, v2.uv, t);
    vtx.normal = Lerp(v1.normal, v2.normal, t);
    vtx.rhw = Lerp(v1.rhw, v2.rhw, t);
    // handle varying
    return std::move(vtx);
}

Vec2i Vertex::ScreenPos() const
{
    return Vec2i(static_cast<i32>(pos.x + 0.5f), static_cast<i32>(pos.y + 0.5f));
}

std::pair<std::pair<Trapezoid, Trapezoid>, u32> Trapezoid::FromPrimitive(Vertex& v1, Vertex& v2, Vertex& v3)
{
    ZoneScopedN("Trapezoid generation");

    /// Sort vertices by y, v1.y < v2.y < v3.y
    Vertex& temp = v1;
    if (v1.pos.y > v2.pos.y) { std::swap(v1, v2); }
    if (v1.pos.y > v3.pos.y) { std::swap(v1, v3); }
    if (v2.pos.y > v3.pos.y) { std::swap(v2, v3); }

    if (v1.pos.y == v2.pos.y && v1.pos.y == v3.pos.y)
    {
        return {{}, 0u};
    }
    if (v1.pos.x == v2.pos.x && v1.pos.x == v3.pos.x)
    {
        return {{}, 0u};
    }

    Trapezoid trap1, trap2;

    /// Down trap
    if (v1.pos.y == v2.pos.y)
    {
        /// Make v1 is the left
        if (v1.pos.x > v2.pos.x) { std::swap(v1, v2); }
        trap1.top = v1.pos.y;
        trap1.bottom = v3.pos.y;
        trap1.left.v1  = &v1;
        trap1.left.v2  = &v3;
        trap1.right.v1 = &v2;
        trap1.right.v2 = &v3;
        /// For precision purposes, another check is needed
        return {{trap1, {}}, (trap1.top < trap1.bottom) ? 1u : 0u};
    }

    /// Up trap
    if (v2.pos.y == v3.pos.y)
    {
        /// Make v2 is the left
        if (v2.pos.x > v3.pos.x) { std::swap(v2, v3); }

        trap1.top = v1.pos.y;
        trap1.bottom = v3.pos.y;
        trap1.left.v1  = &v1;
        trap1.left.v2  = &v2;
        trap1.right.v1 = &v1;
        trap1.right.v2 = &v3;
        /// For precision purposes, another check is needed
        return {{trap1, {}}, (trap1.top < trap1.bottom) ? 1u : 0u};
    }

    trap1.top = v1.pos.y;
    trap1.bottom = v2.pos.y;
    trap2.top = v2.pos.y;
    trap2.bottom = v3.pos.y;

    f32 k = (v3.pos.y - v1.pos.y) / (v2.pos.y - v1.pos.y);
    f32 x = v1.pos.x + k * (v2.pos.x - v1.pos.x);

    if (x <= v3.pos.x) /// v3 is on the right side of v1v2
    {
        trap1.left.v1  = &v1;
        trap1.left.v2  = &v2;
        trap1.right.v1 = &v1;
        trap1.right.v2 = &v3;
        trap2.left.v1  = &v2;
        trap2.left.v2  = &v3;
        trap2.right.v1 = &v1;
        trap2.right.v2 = &v3;
    }
    else /// v3 is on the left side of v1v2
    {
        trap1.left.v1  = &v1;
        trap1.left.v2  = &v3;
        trap1.right.v1 = &v1;
        trap1.right.v2 = &v2;
        trap2.left.v1  = &v1;
        trap2.left.v2  = &v3;
        trap2.right.v1 = &v2;
        trap2.right.v2 = &v3;        
    }
 
    return {{trap1, trap2}, 2u};
}

std::pair<Vertex, Vertex> Trapezoid::LineYEnds(f32 y) const
{
    f32 t1 = (y - left.v1->pos.y) / (left.v2->pos.y - left.v1->pos.y);
    f32 t2 = (y - right.v1->pos.y) / (right.v2->pos.y - right.v1->pos.y);
    Vertex v1 = FromInterpolation(*left.v1, *left.v2, t1);
    Vertex v2 = FromInterpolation(*right.v1, *right.v2, t2);
    return {std::move(v1), std::move(v2)};
}

Scanline Scanline::FromTrapezoid(const Trapezoid& trap, i32 y)
{
    auto [vl, vr] = trap.LineYEnds((f32)y + 0.5f);
    f32 width = vr.pos.x - vl.pos.x;

    Scanline scanline;
    scanline.x = (i32)(vl.pos.x + 0.5f);
    scanline.width = (i32)(vr.pos.x + 0.5f) - scanline.x;
    scanline.y = y;
    scanline.start = std::move(vl);

    f32 inv = 1.0f / width;
    scanline.step.pos = (vr.pos - vl.pos) * inv;
    scanline.step.uv = (vr.uv - vl.uv) * inv;
    scanline.step.normal = (vr.normal - vl.normal) * inv;
    scanline.step.rhw = (vr.rhw - vl.rhw) * inv;

    return std::move(scanline);
}

} // namespace scsr
