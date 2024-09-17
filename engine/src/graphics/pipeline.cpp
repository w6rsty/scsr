#include "graphics/pipeline.hpp"
#include "core/math/vector.hpp"
#include "core/type.hpp"
#include "graphics/obj_loader.hpp"
#include "graphics/vertex.hpp"

#include <Tracy.hpp>

namespace scsr
{

Pipeline::Pipeline()
{}

PrimitiveResult Pipeline::PrimitiveGeneration(Ref<Image> image, std::span<Vertex> vtxs)
{
    ZoneScoped;
    for (auto& vtx : vtxs)
    {
        vtx.pos = m_VertexChanging(vtx);

        // Homogeneous culling
        if (vtx.pos.w <= 1.0f) { return PrimitiveResult::Discard; }
        if (vtx.pos.x <= -vtx.pos.w || vtx.pos.x >= vtx.pos.w) {  return PrimitiveResult::Discard; }
        if (vtx.pos.y <= -vtx.pos.w || vtx.pos.y >= vtx.pos.w) { return PrimitiveResult::Discard; }
        if (vtx.pos.z <= -vtx.pos.w || vtx.pos.z >= vtx.pos.w) { return PrimitiveResult::Discard; }

        vtx.rhw = 1.0f / vtx.pos.w;
        vtx.pos.x *= vtx.rhw;
        vtx.pos.y *= vtx.rhw;
        vtx.pos.z *= vtx.rhw;
        vtx.pos.w = 1.0f;

        // viewport
        vtx.pos.x = (vtx.pos.x + 1.0f) * 0.5f * image->Width();
        vtx.pos.y = (1.0f - vtx.pos.y) * 0.5f * image->Height();
    }
    // Face culling
    if (m_State.cullMode != FaceCullMode::None)
    {
        Vec3 v1 = vtxs[0].pos.xyz();
        Vec3 v2 = vtxs[1].pos.xyz();
        Vec3 v3 = vtxs[2].pos.xyz();
        Vec3 normal = Cross(v2 - v1, v3 - v1);
        f32 dot = Dot(normal, m_Camera->GetFront());
        if (m_State.cullMode == FaceCullMode::CCW && dot < 0.0f) { return PrimitiveResult::Discard; }
        if (m_State.cullMode == FaceCullMode::CW && dot > 0.0f) { return PrimitiveResult::Discard; }
    }

    PrimitiveAssembly(vtxs);
    return PrimitiveResult::Keep;
}

void Pipeline::PrimitiveAssembly(std::span<Vertex> vtxs)
{
    if (vtxs.size() == 3)
    {
        auto trapezoids = Trapezoid::FromPrimitive(vtxs[0], vtxs[1], vtxs[2]);
        switch (trapezoids.second)
        {
        case 1:
            m_DrawBuffer.trapezoids.emplace_back(std::move(trapezoids.first.first));
            break;
        case 2:
            m_DrawBuffer.trapezoids.emplace_back(std::move(trapezoids.first.first));
            m_DrawBuffer.trapezoids.emplace_back(std::move(trapezoids.first.second));
            break;
        }
    }
    // other primitive types
}

void Pipeline::Rasterize(Ref<Image> image, const Trapezoid& trap) const
{
    ZoneScopedN("Draw Trapezoid");
    i32 top = static_cast<i32>(trap.top + 0.5f);
    i32 bottom = static_cast<i32>(trap.bottom + 0.5f);
    for (i32 y = top; y < bottom; ++y)
    {
        if (y >= 0 && y < image->Height())
        {
            Scanline scanline = Scanline::FromTrapezoid(trap, y);
            for (i32 x = scanline.x; x < scanline.x + scanline.width; ++x)
            {
                if (x >= 0 && x < image->Width())
                {
                    scanline.start.pos += scanline.step.pos;
                    scanline.start.uv += scanline.step.uv;
                    scanline.start.normal += scanline.step.normal;
                    scanline.start.rhw += scanline.step.rhw;

                    image->TestDepthAndSetPixel(
                        scanline.start.ScreenPos(),
                        scanline.start.pos.z,
                        ColorToHex(m_PixelShading(scanline.start))    
                    );
                }
                if (x >= image->Width()) { break; }
            }
        }
        if (y >= image->Height()) { break; }
    }
}

void Pipeline::Perform(Ref<Image> image, Mesh& mesh)
{
    ZoneScopedN("Draw call");

    {
        ZoneScopedN("Buffer initialization");
        m_DrawBuffer.trapezoids.clear();
        m_DrawBuffer.vertices.clear();

        m_DrawBuffer.vertices = mesh.vertices;
    }
    std::span<Vertex> vertice_view(m_DrawBuffer.vertices);
    {
        ZoneScopedN("Vertex Pass");
        for (usize i = 0; i < m_DrawBuffer.vertices.size(); i += 3)
        {
            switch(PrimitiveGeneration(image, vertice_view.subspan(i, 3)))
            {
            case PrimitiveResult::Discard:
            case PrimitiveResult::Keep:
                break;
            case PrimitiveResult::Split:
                break;
            }
        }
    }
    {
        ZoneScopedN("Pixel Pass");
        for (auto& trapezoid : m_DrawBuffer.trapezoids)
        {
            Rasterize(image, trapezoid);
        }
    }
    return;
}

}