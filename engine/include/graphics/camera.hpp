#pragma once

#include "core/math/math.hpp"

namespace scsr
{

class Camera
{
public:
    /// VerticalFov in radians
    Camera(f32 verticalFov, f32 aspectRatio, f32 nearClip, f32 farClip);

    void SetPosition(const Vec3& postition);
    void SetOrientation(const Vec3& front, const Vec3& up);

    const Vec3& GetPosition() const { return m_Position; }
    const Vec3& GetFront() const { return m_Font; }
    const Vec3& GetUp() const { return m_Up; }
    const Mat4& GetProjection() const { return m_Projection; }
    const Mat4& GetView() const { return m_View; }
private:
    void UpdateProjection();
    void UpdateView();

    Vec3 m_Position;
    Vec3 m_Font;
    Vec3 m_Up;

    f32 m_VerticalFov;
    f32 m_AspectRatio;
    f32 m_NearClip;
    f32 m_FarClip;

    Mat4 m_Projection;
    Mat4 m_View;
};

}