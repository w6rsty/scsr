#pragma once

#include "core/type.hpp"
#include "core/math/math.hpp"
#include "graphics/image.hpp"
#include "graphics/obj_loader.hpp"
#include "graphics/vertex.hpp"

#include <functional>
#include <span>
#include <vector>

namespace scsr
{

struct DrawBuffer
{
    std::vector<Vertex> vertices;
    std::vector<Trapezoid> trapezoids;
};

using VertexChanging = std::function<Vec4(Vertex&)>;
using PixelShading =  std::function<Vec4(Vertex&)>;

class Pipeline
{
public:
    Pipeline();

    void SetVertexChanging(VertexChanging changing) { m_VertexChanging = changing; }
    void SetFragmentShading(PixelShading shading) { m_PixelShading = shading; }

    void Perform(Ref<Image> image, Mesh& mesh);
private:
    void VertexTransform(Ref<Image> image, std::span<Vertex> vtxs);
    void PrimitiveAssembly(std::span<Vertex> vtxs);
    void Rasterize(Ref<Image> image, const Trapezoid& trap) const;

    // void DrawScanline(Ref<Image> image, const Trapezoid& trap) const;
    
    VertexChanging m_VertexChanging;
    PixelShading m_PixelShading;

    DrawBuffer m_DrawBuffer;
};

}