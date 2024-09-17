#pragma once

#include "core/core.hpp"
#include "camera_controller.hpp"
#include "core/math/vector.hpp"

#include <Tracy.hpp>

using namespace scsr;

static Mesh mesh("assets/meshes/african_head.obj");
static void RenderPlugin(World& world, Storage& storage)
{

    ImageProp prop { .width = 800, .height = 600 };
    
    world.RegisterObject<Pipeline>();
    world.RegisterObject<Swapchain>(prop, 1);

    auto& pipeline = storage.GetObject<Pipeline>();
    auto& swapchain = storage.GetObject<Swapchain>();
    auto& camera = storage.GetObject<CameraController>().cam;
    pipeline.SetCamera(camera);
    
    // Set shaders
    pipeline.SetVertexChanging([&](Vertex& vtx) -> Vec4 {
        ZoneScopedN("Vertex changing");
        Vec4 position = camera->GetProjection() * camera->GetView() * vtx.pos;
        return position;
    });
    pipeline.SetFragmentShading([](Vertex&vtx) -> Vec4 {
        f32 theta = Abs(Dot(vtx.normal, Vec3::Z()));
        return Vec4(Vec3::ONE() * theta, 1.0f);
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