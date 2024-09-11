#include "graphics/pipeline.hpp"
#include "graphics/vertex.hpp"

#include <optional>

#include <Tracy.hpp>

namespace scsr
{

Pipeline::Pipeline()
{}

std::optional<Primitive> Pipeline::VertexTransform(Ref<Image> image, Primitive& primitive)
{
    ZoneScoped;
    {
        ZoneScopedN("Vertex Processing");
        for (usize i = 0; i < 3; ++i)
        {
            Vertex& vtx = primitive[i];
            
            // Call vertex changing function
            Vec4 out = m_VertexChanging(vtx, vtx.varyings);

            // Homogeneous culling
            if (out.w == 0.0) { return std::nullopt; }
            if (out.x < -out.w || out.x > out.w) {  return std::nullopt; }
            if (out.y < -out.w || out.y > out.w) { return std::nullopt; }
            if (out.z < -out.w || out.z > out.w) { return std::nullopt; }

            // save 1/w for perspective correction
            vtx.rhw = 1.0f / out.w;
            // Perspective division
            out *= vtx.rhw;

            // viewport transformation
            vtx.spf.x = (out.x + 1.0f) * 0.5f * (image->Width());
            vtx.spf.y = (1.0f - out.y) * 0.5f * (image->Height());

            vtx.spi = Vec2i(static_cast<i32>(vtx.spf.x + 0.5f), static_cast<i32>(vtx.spf.y + 0.5f));
        }
    }
    return primitive;
}

void Pipeline::Rasterize(Ref<Image> image, const Trapezoid& trap)
{
    // Edage Walking implementation
    ZoneScopedN("Rasterization");
}

void Pipeline::Perform(Ref<Image> image, Mesh& mesh)
{
    ZoneScopedN("Draw call");

    DrawBuffer buffer;
    {
        ZoneScopedN("Vertex Transformation");

        for (auto& face : mesh.faces)
        {
            Primitive prim;
            prim[0].pos = mesh.vertices.at(face[0].x);
            prim[1].pos = mesh.vertices.at(face[1].x);
            prim[2].pos = mesh.vertices.at(face[2].x);

            auto result = VertexTransform(image, prim);
            if (!result.has_value()) {  continue; }

            auto trapezoids = Trapezoid::FromPrimitive(prim);
            if (!trapezoids[0].has_value()) { continue; }

            buffer.trapezoids.push_back(trapezoids[0].value());
            buffer.trapezoids.push_back(trapezoids[1].value());
        }
    }

    {
        ZoneScopedN("Rasterization");
        for (auto& trapezoid : buffer.trapezoids)
        {
            Rasterize(image, trapezoid);
        }
    }

    return;
}

}