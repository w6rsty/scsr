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

void Pipeline::VertexTransform(Ref<Image> image, std::span<Vertex> vtxs)
{
    ZoneScoped;
    {
        ZoneScopedN("Primitive Transform");
        for (auto& vtx : vtxs)
        {
            // Call vertex changing function
            Vec4 out = m_VertexChanging(vtx);

            // Homogeneous culling
            if (out.w == 0.0) { return; }
            if (out.x < -out.w || out.x > out.w) {  return; }
            if (out.y < -out.w || out.y > out.w) { return; }
            if (out.z < -out.w || out.z > out.w) { return; }

            vtx.rhw = 1.0f / out.w;

            vtx.pos.x = (out.x * vtx.rhw + 1.0f) * 0.5f * (image->Width() - 1);
            vtx.pos.y = (1.0f - out.y * vtx.rhw) * 0.5f * (image->Height() - 1);
            vtx.pos.z = out.z * vtx.rhw;

            vtx.uv *= vtx.rhw;
            vtx.normal *= vtx.rhw;
        }
    }

    PrimitiveAssembly(vtxs);
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

                    Vec2i screen_pos = scanline.start.ScreenPos();

                    u32 color_u32 = ColorToHex(m_PixelShading(scanline.start));
                    image->SetPixel(screen_pos, color_u32);
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
            VertexTransform(image, vertice_view.subspan(i, 3));
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