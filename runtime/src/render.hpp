#pragma once

#include "camera.hpp"
#include "core/core.hpp"
#include "core/type.hpp"

using namespace scsr;

static Vec3 positions[6] = {
    Vec3(-0.5f, -0.5f,  -1.0f), // 0
    Vec3( 0.5f, -0.5f,  -1.0f), // 1
    Vec3(-0.5f,  0.5f,  -1.0f), // 3
    Vec3(-0.5f,  0.5f,  -1.0f), // 3
    Vec3( 0.5f, -0.5f,  -1.0f), // 1
    Vec3( 0.5f,  0.5f,  -1.0f), // 2
};

static Vec3 colors[6] = {
    Vec3(1.0f, 0.0f, 0.0f),
    Vec3(0.0f, 1.0f, 0.0f),
    Vec3(0.0f, 0.0f, 1.0f),
    Vec3(0.0f, 0.0f, 1.0f),
    Vec3(0.0f, 1.0f, 0.0f),
    Vec3(0.0f, 1.0f, 1.0f)
};

static f32 s_Time = 0.0f;

static void RenderPlugin(World& world, Storage& storage)
{
    world.RegisterObject<Pipeline>();

    auto& pipeline = storage.GetObject<Pipeline>();
    // Set vertex
    pipeline.GetVertexDescription().SetCount(6);
    pipeline.GetVertexDescription().SetPositions(positions);
    pipeline.GetVertexDescription().SetColors(colors);

    // Set shaders
    pipeline.SetVertexChanging([&](VertexView vv, Varyings& varyings) -> Vec4 {
        auto& pos = vv.pos;

        auto& camera = storage.GetObject<Camera>();

        s_Time += storage.GetObject<Ticker>().delta / 1000.0f;
        
        auto model = FromRotation(Quat::FromAxisAngle(Vec3::Y(), s_Time * 0.2f));
        Vec4 position = camera.proj * camera.view * model * Vec4(pos, 1.0f);

        varyings.vec2[0] = pos.xy();

        return position;
    });
    pipeline.SetFragmentShading([](Varyings& varyings) -> Vec4 {
        auto uv = varyings.vec2[0] * 0.5f + Vec2::Splat(0.5f);

        return Vec4(uv,  0.0f, 1.0f);
    });   
}