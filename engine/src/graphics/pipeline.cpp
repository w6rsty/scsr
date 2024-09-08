#include "graphics/pipeline.hpp"
#include "core/io.hpp"

namespace scsr
{

Pipeline::Pipeline(usize threads) :
    m_Threads(threads)
{}

void Pipeline::DrawPrimitive(usize offset, VertexDescription vd, void* ptr, usize pitch)
{
    for (usize i = 0; i < 3; ++i)
    {
        VertexView& vv = m_VertexView[i];
        vv.pos = vd.m_Positions[offset + i];
        vv.color = vd.m_Colors[offset + i];

        vv.varyings.Reset();

        // Call vertex changing function
        Vec4 out = m_VertexChanging(vv, vv.varyings);

        // Homogeneous culling
        if (out.w == 0.0)
        { 
            return;
        }
        if (out.x < -out.w || out.x > out.w)
        { 
            return; 
        }
        if (out.y < -out.w || out.y > out.w) {
            return;
        }
        if (out.z < -out.w || out.z > out.w) {
            return;
        }

        // save 1/w for perspective correction
        vv.recipW = 1.0f / out.w;
        // Perspective division
        out *= vv.recipW;

        // viewport transformation
        vv.spf.x = (out.x + 1.0f) * 0.5f * (800 + 1);
        vv.spf.y = (1.0f - out.y) * 0.5f * (600 + 1);

        vv.x = static_cast<i32>(vv.spf.x + 0.5f);
        vv.y = static_cast<i32>(vv.spf.y + 0.5f);
    }

    i32 minX = Min<i32>(m_VertexView[0].x, Min<i32>(m_VertexView[1].x, m_VertexView[2].x));
    i32 minY = Min<i32>(m_VertexView[0].y, Min<i32>(m_VertexView[1].y, m_VertexView[2].y));
    i32 maxX = Max<i32>(m_VertexView[0].x, Max<i32>(m_VertexView[1].x, m_VertexView[2].x));
    i32 maxY = Max<i32>(m_VertexView[0].y, Max<i32>(m_VertexView[1].y, m_VertexView[2].y));    

    // Rasterization
    // Edge equations
    for (i32 ry = minY; ry < maxY; ++ry)
    {
        for (i32 rx = minX; rx < maxX; ++rx)
        {
            Vec2 p = Vec2((f32)rx + 0.5f, (f32)ry + 0.5f);

            Vec2 v0 = m_VertexView[1].spf - m_VertexView[0].spf; // B-A
            Vec2 v1 = m_VertexView[2].spf - m_VertexView[0].spf; // C-A
            Vec2 v2 = p - m_VertexView[0].spf; // P-A

            f32 dot00 = Dot(v0, v0);
            f32 dot01 = Dot(v0, v1);
            f32 dot11 = Dot(v1, v1);
            f32 dot20 = Dot(v2, v0);
            f32 dot21 = Dot(v2, v1);

            f32 recipDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
            f32 b = (dot11 * dot20 - dot01 * dot21) * recipDenom;
            f32 c = (dot00 * dot21 - dot01 * dot20) * recipDenom;
            f32 a = 1.0f - b - c;


            if (a > 0.0f && b > 0.0f && c > 0.0f)
            {
                Varyings varyings;

                for (usize i = 0; i < MaxVaryingNum; ++i)
                {
                    varyings.vec2[i] = m_VertexView[0].varyings.vec2[i] * a + m_VertexView[1].varyings.vec2[i] * b + m_VertexView[2].varyings.vec2[i] * c;
                    varyings.vec3[i] = m_VertexView[0].varyings.vec3[i] * a + m_VertexView[1].varyings.vec3[i] * b + m_VertexView[2].varyings.vec3[i] * c;
                    varyings.vec4[i] = m_VertexView[0].varyings.vec4[i] * a + m_VertexView[1].varyings.vec4[i] * b + m_VertexView[2].varyings.vec4[i] * c;
                }

                // Call pixel shading function
                Vec4 color = m_PixelShading(varyings);
                // Vec4 color = Vec4(m_VertexView[0].color * a + m_VertexView[1].color * b + m_VertexView[2].color * c, 1.0f);
                u32 out = u32(color.w * 255) << 24
                    | u32(color.z * 255) << 16
                    | u32(color.y * 255) << 8
                    | u32(color.x * 255);
                // Write to framebuffer
                *reinterpret_cast<u32*>(static_cast<u8*>(ptr) + ry * pitch + rx * 4) = out;
            }
        }
    }
}

void Pipeline::Perform(void* ptr, usize pitch)
{
    PROFILE_SCOPE("Pipeline::Perform");


    DrawPrimitive(0, m_VertexDescription, ptr, pitch);
    DrawPrimitive(3, m_VertexDescription, ptr, pitch);
}

}