#pragma once

#include "core/math/math.hpp"
#include "core/math/vector.hpp"
#include "core/type.hpp"

#include <utility>

namespace scsr
{

class Vertex
{
public:
    static Vertex FromInterpolation(const Vertex& v1, const Vertex& v2, f32 t);
    Vec2i ScreenPos() const;

    Vec3 pos;
    Vec2 uv;
    Vec3 normal;
    f32 rhw;
};

/// A view of vertices, be careful with lifetime
struct Edge {
    Vertex* v1 = nullptr;
    Vertex* v2 = nullptr;
};

struct Trapezoid
{
    f32 top;
    f32 bottom;
    Edge left;
    Edge right;

    static std::pair<std::pair<Trapezoid, Trapezoid>, u32> FromPrimitive(Vertex& v1, Vertex& v2, Vertex& v3);
    std::pair<Vertex, Vertex> LineYEnds(f32 y) const; 
};

struct Scanline
{
    Vertex start;
    Vertex step;
    i32 x;
    i32 y;
    i32 width;

    static Scanline FromTrapezoid(const Trapezoid& trap, i32 y);
};
    
};