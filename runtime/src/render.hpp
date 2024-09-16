#pragma once

#include "camera.hpp"
#include "core/core.hpp"
#include "core/math/def.hpp"
#include "core/math/ext.hpp"
#include "core/math/quaternion.hpp"
#include "core/math/vector.hpp"
#include "core/type.hpp"
#include "graphics/swapchain.hpp"

#include "graphics/obj_loader.hpp"

#include <Tracy.hpp>

using namespace scsr;

static f32 s_Time = 0.0f;

static Mesh mesh("assets/meshes/african_head.obj");
static void RenderPlugin(World& world, Storage& storage)
{

    ImageProp prop { .width = 800, .height = 600 };
    
    world.RegisterObject<Pipeline>();
    world.RegisterObject<Swapchain>(prop, 1);

    auto& pipeline = storage.GetObject<Pipeline>();
    auto& swapchain = storage.GetObject<Swapchain>();
    
    // Set shaders
    pipeline.SetVertexChanging([&](Vertex& vtx) -> Vec4 {
        ZoneScopedN("Vertex Changing");
        auto& pos = vtx.pos;

        auto& camera = storage.GetObject<Camera>();

        s_Time += storage.GetObject<Ticker>().delta / 1000.0f;

        auto model = FromRotation(Quat::FromAxisAngle(Vec3::Y(), s_Time * 0.0001f));
        
        Vec4 position = camera.proj * camera.view * model * Vec4(pos, 1.0f);

        return position;
    });
    pipeline.SetFragmentShading([](Vertex&vtx) -> Vec4 {
        Vec3 albedo = ColorFromHex(0x483A25).xyz();
        static Vec3 light_dir = Vec3(0.0f, 0.0f, 1.0f);
        Vec3 normal = Normalized(vtx.normal);
        f32 intensity = Min(1.0f, Max(0.0f, Dot(normal, light_dir)));
        return Vec4(albedo * intensity, 1.0f);
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