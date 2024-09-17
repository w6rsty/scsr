#pragma once

#include "core/type.hpp"
#include "core/math/math.hpp"
#include "graphics/image.hpp"
#include "graphics/obj_loader.hpp"
#include "graphics/vertex.hpp"
#include "graphics/camera.hpp"

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

enum class FaceCullMode
{
    None,
    CCW,
    CW
};

struct PipelineState {
    FaceCullMode cullMode = FaceCullMode::CCW;
};

enum class PrimitiveResult
{
    Discard,
    Keep,
    Split
};

using VertexChanging = std::function<Vec4(Vertex&)>;
using PixelShading =  std::function<Vec4(Vertex&)>;

class Pipeline
{
public:
    Pipeline();

    void SetCamera(Ref<Camera>& camera) { m_Camera = camera; }
    void SetVertexChanging(VertexChanging changing) { m_VertexChanging = changing; }
    void SetFragmentShading(PixelShading shading) { m_PixelShading = shading; }

    void Perform(Ref<Image> image, Mesh& mesh);
private:
    PrimitiveResult PrimitiveGeneration(Ref<Image> image, std::span<Vertex> vtxs);
    void PrimitiveAssembly(std::span<Vertex> vtxs);
    void Rasterize(Ref<Image> image, const Trapezoid& trap) const;

    // void DrawScanline(Ref<Image> image, const Trapezoid& trap) const;
    
    Ref<Camera> m_Camera;
    VertexChanging m_VertexChanging;
    PixelShading m_PixelShading;

    DrawBuffer m_DrawBuffer;

    PipelineState m_State;
};

}