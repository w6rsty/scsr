#include "graphics/camera.hpp"
#include "core/math/ext.hpp"

namespace scsr
{

Camera::Camera(f32 VerticalFov, f32 aspecRatio, f32 nearClip, f32 farClip) :
    m_VerticalFov(VerticalFov),
    m_AspectRatio(aspecRatio),
    m_NearClip(nearClip),
    m_FarClip(farClip),
    m_Position(Vec3::ZERO()),
    m_Font(Vec3::NEG_Z()),
    m_Up(Vec3::Y()),
    m_Projection(Mat4::IDENTITY()),
    m_View(Mat4::IDENTITY())
{
    UpdateProjection();
    UpdateView();
}

void Camera::SetPosition(const Vec3& position)
{
    m_Position = position;
    UpdateView();
}

void Camera::SetOrientation(const Vec3& front, const Vec3& up)
{
    m_Font = front;
    m_Up = up;
    UpdateView();
}

void Camera::UpdateProjection()
{
    m_Projection = ProjectionPerspective(m_VerticalFov, m_AspectRatio, m_NearClip, m_FarClip);
}

void Camera::UpdateView()
{
    m_View = LookAt(m_Position, m_Position + m_Font, m_Up);
}

}