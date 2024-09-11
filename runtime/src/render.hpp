#pragma once

#include "camera.hpp"
#include "core/core.hpp"
#include "core/type.hpp"
#include "graphics/swapchain.hpp"

#include "graphics/obj_loader.hpp"

#include <Tracy.hpp>

using namespace scsr;

static Vec3 positions[] = {
    Vec3(-0.5f, -0.5f,  -1.0f), // 0
    Vec3( 0.5f, -0.5f,  -1.0f), // 1
    Vec3(-0.5f,  0.5f,  -1.0f), // 3
    Vec3(-0.5f,  0.5f,  -1.0f), // 3
    Vec3( 0.5f, -0.5f,  -1.0f), // 1
    Vec3( 0.5f,  0.5f,  -1.0f), // 2
};

static Vec3 colors[] = {
    Vec3(1.0f, 0.0f, 0.0f),
    Vec3(0.0f, 1.0f, 0.0f),
    Vec3(0.0f, 0.0f, 1.0f),
    Vec3(0.0f, 0.0f, 1.0f),
    Vec3(0.0f, 1.0f, 0.0f),
    Vec3(0.0f, 1.0f, 1.0f),
};

static f32 s_Time = 0.0f;

static Mesh mesh("assets/meshes/african_head.obj");
static void RenderPlugin(World& world, Storage& storage)
{

    ImageProp prop { .width = 800, .height = 600 };
    
    world.RegisterObject<Pipeline>();
    world.RegisterObject<Swapchain>(prop, 2);

    auto& pipeline = storage.GetObject<Pipeline>();
    auto& swapchain = storage.GetObject<Swapchain>();
    
    // Set shaders
    pipeline.SetVertexChanging([&](Vertex& vtx, Varyings& varyings) -> Vec4 {
        ZoneScopedN("Vertex Changing");
        auto& pos = vtx.pos;

        auto& camera = storage.GetObject<Camera>();

        s_Time += storage.GetObject<Ticker>().delta / 1000.0f;
        
        // auto model = FromRotation(Quat::FromAxisAngle(Vec3::Y(), s_Time * 0.001f));
        Vec4 position = camera.proj * camera.view * Vec4(pos, 1.0f);

        varyings.vec2[0] = pos.xy();

        return position;
    });
    pipeline.SetFragmentShading([](Varyings& varyings) -> Vec4 {
        ZoneScopedN("Fragment Shading");
        auto uv = varyings.vec2[0] * 0.5f + Vec2::Splat(0.5f);

        return Vec4(uv, 0.0f, 1.0f);
    });

    swapchain.PushWriteCommand([&](Ref<Image> image) {
        image->Clear();
    });
    swapchain.PushWriteCommand([&](Ref<Image> image) {
        pipeline.Perform(image, mesh);
    });

    world.AddSystem([](Storage& storage) {
        auto& swapchain = storage.GetObject<Swapchain>();
        auto& window = storage.GetObject<Window>();

        swapchain.AcquireAndWrite();
        swapchain.Present(window);
    });
}