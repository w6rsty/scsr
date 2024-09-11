#pragma once

#include "core/math/math.hpp"

#include <array>

namespace scsr
{

enum class Attribute
{
    Normal,
    Tangent,
    Bitangent,
    TexCoord,
    Color
};

struct AttributeInfo
{
    Attribute type;
    Vec4 data;
};

constexpr usize MaxVaryingNum = 4;

struct Varyings
{
    std::array<Vec2, MaxVaryingNum> vec2;
    std::array<Vec3, MaxVaryingNum> vec3;
    std::array<Vec4, MaxVaryingNum> vec4;

    void Reset()
    {
        for (usize i = 0; i < MaxVaryingNum; ++i)
        {
            vec2[i] = Vec2::ZERO();
            vec3[i] = Vec3::ZERO();
            vec4[i] = Vec4::ZERO();
        }
    }
};  

class Vertex
{
public:
    void PushAttribute(Attribute type, Vec4 data);
    static Vertex Interpolated(const Vertex& v0, const Vertex& v1, f32 t);

    Vec3 pos;
    std::vector<AttributeInfo> attributes;
private:
    friend class Pipeline;
    Vec2 spf;
    Vec2i spi;
    f32 rhw;
    Varyings varyings;
};

struct Primitive
{  
    Vertex vertices[3];
    Vertex& operator[](usize index);
};

class Trapezoid
{
public:
    struct Edge
    {
        Edge(Vertex& v0, Vertex& v1);
        Vertex& v0;
        Vertex& v1;
    };

    Trapezoid(f32 top, f32 bottom, Edge left, Edge right);
    static std::array<std::optional<Trapezoid>, 2> FromPrimitive(Primitive& prim);

    f32 Top() const { return m_Top; }
    f32 Bottom() const { return m_Bottom; }
    Edge Left() const { return m_Left; }
    Edge Right() const { return m_Right; }
private:
    friend class Pipeline;
    f32 m_Top;
    f32 m_Bottom;
    Edge m_Left;
    Edge m_Right;
};

class Scanline
{
public:
    Scanline(Vertex vertex, Vertex step, f32 y, f32 width);
    static Scanline FromTrapezoid(Trapezoid& trapezoid, f32 y);
private:
    friend class Pipeline;
    Vertex m_Vertex;
    Vertex m_Step;
    f32 m_Y;
    f32 m_Width;
};

    
};