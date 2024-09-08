#pragma once

#include "core/math/vector.hpp"
#include "core/type.hpp"
#include "core/math/math.hpp"

#include <array>
#include <functional>

namespace scsr
{

using AttachementID = usize;

class VertexDescription
{
    friend class Pipeline;
public:
    VertexDescription() = default;
    VertexDescription(usize count) :
        m_Count(count),
        m_Positions(nullptr),
        m_Colors(nullptr) 
    {}

    void SetPositions(Vec3* positions) { m_Positions = positions; }
    void SetColors(Vec3* colors) { m_Colors = colors; }

    void SetCount(usize count) { m_Count = count; }
    usize GetCount() const { return m_Count; }
private:
    usize m_Count;
    Vec3* m_Positions;
    Vec3* m_Colors;
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

/// A collection of vertex attributes
class VertexView
{
public:
    Vec3 pos;
    Vec3 color;
private:
    friend Pipeline;

    f32 recipW;
    Vec2 spf;
    i32 x;
    i32 y;
    Varyings  varyings;
};

using VertexChanging = std::function<Vec4(VertexView, Varyings&)>;
using PixelShading =  std::function<Vec4(Varyings&)>;

class Pipeline
{
public:
    Pipeline(usize threads = 4);

    void SetVertexChanging(VertexChanging changing) { m_VertexChanging = changing; }
    void SetFragmentShading(PixelShading shading) { m_PixelShading = shading; }

    VertexDescription& GetVertexDescription() { return m_VertexDescription; }

    void DrawPrimitive(usize offset, VertexDescription vd, void* ptr, usize pitch);
    void Perform(void* ptr, usize pitch);
private:
    usize m_Threads;
    VertexDescription m_VertexDescription;
    VertexView m_VertexView[3];
    VertexChanging m_VertexChanging;
    PixelShading m_PixelShading;
};

}