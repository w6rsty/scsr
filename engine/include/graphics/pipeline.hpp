#pragma once

#include "core/type.hpp"
#include "core/math/math.hpp"
#include "graphics/image.hpp"
#include "graphics/obj_loader.hpp"
#include "graphics/vertex.hpp"

#include <functional>

namespace scsr
{

struct DrawBuffer
{
    std::vector<Trapezoid> trapezoids;
};

using VertexChanging = std::function<Vec4(Vertex&, Varyings&)>;
using PixelShading =  std::function<Vec4(Varyings&)>;

class Pipeline
{
public:
    Pipeline();

    void SetVertexChanging(VertexChanging changing) { m_VertexChanging = changing; }
    void SetFragmentShading(PixelShading shading) { m_PixelShading = shading; }

    std::optional<Primitive> VertexTransform(Ref<Image> image, Primitive& primitive);
    void Perform(Ref<Image> image, Mesh& mesh);
    void Rasterize(Ref<Image> image, const Trapezoid& trap);
private:
    VertexChanging m_VertexChanging;
    PixelShading m_PixelShading;
};

}