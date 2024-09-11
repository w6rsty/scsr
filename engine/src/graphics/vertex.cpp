#include "graphics/vertex.hpp"
#include "core/math/def.hpp"

#include <algorithm>

#include <Tracy.hpp>

namespace scsr
{

void Vertex::PushAttribute(Attribute type, Vec4 data)
{
    attributes.push_back({type, data});
}

Vertex Vertex::Interpolated(const Vertex &v0, const Vertex &v1, f32 t)
{
    ZoneScopedN("Vertex interpolation");
    Vertex result;
    result.pos = Lerp(v0.pos, v1.pos, t);
    result.rhw = Lerp(v0.rhw, v1.rhw, t);
    result.spf = Lerp(v0.spf, v1.spf, t);
    result.spi = Vec2i(static_cast<i32>(result.spf.x + 0.5f), static_cast<i32>(result.spf.y + 0.5f));

    for (usize i = 0; i < v0.attributes.size(); ++i)
    {
        Vec4 data;
        switch (v0.attributes[i].type)
        {
        case Attribute::TexCoord:
            data = Vec4(Lerp(v0.attributes[i].data.xy(), v1.attributes[i].data.xy(), t), 0.0f, 0.0f);
            break;
        case Attribute::Normal:
        case Attribute::Tangent:
        case Attribute::Bitangent:
            data = Vec4(Lerp(v0.attributes[i].data.xyz(), v1.attributes[i].data.xyz(), t), 0.0f);
            break;
        case Attribute::Color:
            data = Vec4(Lerp(v0.attributes[i].data.xyz(), v1.attributes[i].data.xyz(), t), v0.attributes[i].data.w);
            break;
        }
        result.PushAttribute(v0.attributes[i].type, data);
    }

    return result;
}

Vertex& Primitive::operator [] (usize index)
{
    return vertices[index];
}

Trapezoid::Edge::Edge(Vertex& v0, Vertex& v1) : v0(v0), v1(v1) {}

Trapezoid::Trapezoid(f32 top, f32 bottom, Edge left, Edge right) : 
    m_Top(top), m_Bottom(bottom), m_Left(left), m_Right(right)
{
}

std::array<std::optional<Trapezoid>, 2> Trapezoid::FromPrimitive(Primitive& prim)
{
    ZoneScopedN("Trapezoid generation");
    /// Sort vertices by y from top to bottom
    std::sort(prim.vertices, prim.vertices + 3, [](const Vertex& lhs, const Vertex& rhs) {
        return lhs.pos.y < rhs.pos.y;
    });

    Vertex& v0 = prim.vertices[0];
    Vertex& v1 = prim.vertices[1];
    Vertex& v2 = prim.vertices[2];

    /// Upper triangle
    if (v1.pos.y == v0.pos.y)
    {
        Trapezoid trap(v0.pos.y, v2.pos.y, Edge(v0, v1), Edge(v0, v2));
        return {trap, std::nullopt};
    }

    /// Lower triangle
    if (v1.pos.y == v2.pos.y)
    {
        Trapezoid trap(v0.pos.y, v2.pos.y, Edge(v0, v2), Edge(v0, v1));
        return {trap, std::nullopt};
    }

    f32 t = (v1.pos.y - v0.pos.y) / (v2.pos.y - v0.pos.y);
    Vertex middle = Vertex::Interpolated(v0, v2, t);

    return {
        Trapezoid(v0.pos.y, v1.pos.y, Edge(v0, v1), Edge(v0, middle)),
        Trapezoid(v1.pos.y, v2.pos.y, Edge(v1, v2), Edge(middle, v2))
    };
}

Scanline::Scanline(Vertex vertex, Vertex step, f32 y, f32 width) :
    m_Vertex(vertex), m_Step(step), m_Y(y), m_Width(width)
{
}

Scanline Scanline::FromTrapezoid(Trapezoid &trapezoid, f32 y)
{
    
}


} // namespace scsr
