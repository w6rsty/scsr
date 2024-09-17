#pragma once

#include "core/core.hpp"
#include "core/math/quaternion.hpp"

using namespace scsr;

struct CameraController
{
    Ref<Camera> cam;

    f32 moveSpeed = 0.01f;
    f32 rotateSpeed = 0.001f;

    CameraController(f32 verticalFov, f32 aspectRatio, f32 nearClip, f32 farClip) :
        cam(MakeRef<Camera>(verticalFov, aspectRatio, nearClip, farClip))
    {
    }

    void MoveTo(const Vec3& position)
    {
        cam->SetPosition(position);
    }

    void Move(const Vec3& delta)
    {
        Vec3 front = -cam->GetFront();
        Vec3 up = cam->GetUp();
        Vec3 right = -Normalized(Cross(front, up));

        cam->SetPosition(cam->GetPosition() + front * delta.z * moveSpeed);
        cam->SetPosition(cam->GetPosition() + right * delta.x * moveSpeed);
        cam->SetPosition(cam->GetPosition() + up * delta.y * moveSpeed);
    }

    void Rotate(f32 dx, f32 dy)
    {
        Vec3 front = cam->GetFront();
        Vec3 up = cam->GetUp();
        Vec3 right = Normalized(Cross(front, up));

        RotationAxisAngle(front, up, dx * rotateSpeed);
        RotationAxisAngle(right, up, dy * rotateSpeed);

        up = Normalized(Cross(right, front));

        cam->SetOrientation(front, up);
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
    storage.GetObject<CameraController>().Move(delta * deltaT);
}

static void MouseMotionInput(Event event, Storage& storage)
{
    f32 deltaT = storage.GetObject<Ticker>().delta;
    storage.GetObject<CameraController>().Rotate(
        -event.mouseMotion.dx, 
        -event.mouseMotion.dy
    );
}

static void CameraControllerPlugin(World& world, Storage& storage)
{
    world.RegisterObject<CameraController>(Radians(30.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    world.RegisterEvent<KeyboardPressedEvent>(KeyInput);
    // world.RegisterEvent<MouseMotionEvent>(MouseMotionInput);

    auto& camera = storage.GetObject<CameraController>();
    camera.MoveTo(Vec3(0, 0, 5));
}