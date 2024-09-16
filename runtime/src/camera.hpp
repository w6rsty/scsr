#pragma once

#include "core/core.hpp"
#include "core/math/quaternion.hpp"

using namespace scsr;

struct Camera
{
    Vec3 position;
    Vec3 rotation;
    Vec3 up;
    Mat4 view;

    f32 verticalFov;
    f32 aspectRatio;
    Mat4 proj;

    f32 moveSpeed = 0.01f;
    f32 rotateSpeed = 0.001f;

    Camera()
    {
        position = Vec3(0.0f, 0.0f, 0.0f);
        rotation = Vec3::NEG_Z();
        up = Vec3::Y();
        verticalFov = Radians(30.0f);
        aspectRatio = 800.0f / 600.0f;
        proj = ProjectionPerspective(verticalFov, aspectRatio, 0.1f, 100.0f);
        UpdateView();
        UpdateProj();
    }

    void SetProjection(f32 fov, f32 aspect, f32 near, f32 far)
    {
        verticalFov = Radians(fov);
        aspectRatio = aspect;
        proj = ProjectionPerspectiveGL(verticalFov, aspectRatio, near, far);
    }
    void Goto(const Vec3& pos)
    {
        position = pos;
        UpdateView();
    }
    void Move(const Vec3& delta)
    {
        position += delta * moveSpeed;
        UpdateView();
    }
    void Rotate(i32 dx, i32 dy)
    {
        Vec2 delta = -Vec2(dx, dy) * rotateSpeed;
        RotationY(rotation, delta.x);
        RotationX(rotation, delta.y);
        UpdateView();
    }
    void UpdateView()
    {   
        view = LookAt(position, position + rotation, up);
    }
    void UpdateProj()
    {
        proj = ProjectionPerspective(verticalFov, aspectRatio, 0.1f, 100.0f);
    }
};

static void KeyInput(Event event, Storage& storage)
{
    Vec3 delta = Vec3::ZERO();
    auto key = event.keyboardPressed.keyCode;
    if (key == KeyboardKeyCode::KeyW)
    {
        delta.z = -1.0f;
    }
    else if (key == KeyboardKeyCode::KeyS)
    {
        delta.z = 1.0f;
    }
    else if (key == KeyboardKeyCode::KeyA)
    {
        delta.x = -1.0f;
    }
    else if (key == KeyboardKeyCode::KeyD)
    {
        delta.x = 1.0f;
    }
    else if (key == KeyboardKeyCode::KeyLeftShift)
    {
        delta.y = 1.0f;
    }
    else if (key == KeyboardKeyCode::KeyLeftControl)
    {
        delta.y = -1.0f;
    } 
    else
    {
        return;
    }
    
    f32 deltaT = storage.GetObject<Ticker>().delta;
    storage.GetObject<Camera>().Move(delta * deltaT);
}

static void MouseMotionInput(Event event, Storage& storage)
{
    f32 deltaT = storage.GetObject<Ticker>().delta;
    storage.GetObject<Camera>().Rotate(event.mouseMotion.dx * deltaT, event.mouseMotion.dy * deltaT);
}

static void CameraPlugin(World& world, Storage& storage)
{
    world.RegisterObject<Camera>();
    world.RegisterEvent<KeyboardPressedEvent>(KeyInput);
    // world.RegisterEvent<MouseMotionEvent>(MouseMotionInput);

    auto& camera = storage.GetObject<Camera>();
    camera.Goto(Vec3(0, 0, 5));
}