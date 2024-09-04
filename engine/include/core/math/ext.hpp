#pragma once

#include "core/assert.hpp"
#include "def.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"

#include <tuple>

namespace scsr
{

inline Mat3 Mat3FromScale(const Vec3& scale)
{
    return Mat3 {
        scale.x, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f,
        0.0f, 0.0f, scale.z
    };
}

inline Mat3 Mat3FromRotation(const Quat& quat)
{
    return quat.ToMat3();
}

inline Mat3 Mat3FromRotation(const Vec3& euler)
{
    return Quat::FromRotationEuler(euler).ToMat3();
}

inline Mat4 Mat4FromScale(const Vec3& scale)
{
    return Mat4 {
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

inline Mat4 Mat4FromRotation(const Quat& quat)
{
    return quat.ToMat4();
}

inline Mat4 Mat4FromRotation(const Vec3& euler)
{
    return Quat::FromRotationEuler(euler).ToMat4();
}

inline Mat4 Mat4FromTranslation(const Vec3& translation)
{
    return Mat4 {
        1.0f, 0.0f, 0.0f, translation.x,
        0.0f, 1.0f, 0.0f, translation.y,
        0.0f, 0.0f, 1.0f, translation.z,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

inline Mat4 Mat4FromScaleRotationTranslation(const Vec3& scale, const Quat& rotation, const Vec3& translation)
{
    Mat3 rotation_mat = rotation.ToMat3();
    return Mat4 {
        Vec4(rotation_mat.col0 * scale.x, 0.0f),
        Vec4(rotation_mat.col1 * scale.y, 0.0f),
        Vec4(rotation_mat.col2 * scale.z, 0.0f),
        Vec4(translation, 1.0f)  
    };
}

/// Decompose a 4x4 matrix into scale, rotation and translation
inline std::tuple<Vec3, Quat, Vec3> Mat4ToScaleRotationTranslation(const Mat4& mat)
{
    f32 det = mat.Determinant();
    RT_ASSERT(det != 0.0f, "Matrix is singular");

    Vec3 scale {
        mat.col0.Length() * scsr::Signum(det),
        mat.col1.Length(),
        mat.col2.Length()
    };

    RT_ASSERT(!scale.IsZero(), "Decomposed scale is zero, cause division by zero");
    Vec3 inv_scale = scale.Recip();

    Quat rotation = Quat::FromMat3(Mat3 {
        (mat.col0 * inv_scale.x).Truncate(),
        (mat.col1 * inv_scale.y).Truncate(),
        (mat.col2 * inv_scale.z).Truncate()
    });

    Vec3 translation = mat.col3.Truncate();

    return { scale, rotation, translation };
}


/// Right-handed Orthgraphic projection matrix
inline Mat4 Mat4Orthographic(
    f32 left, 
    f32 right, 
    f32 bottom, 
    f32 top, 
    f32 near, 
    f32 far
)
{
    f32 a = 2.0f / (right - left);
    f32 b = 2.0f / (top - bottom);
    f32 c = -2.0f / (far - near);
    f32 tx = -(right + left) / (right - left);
    f32 ty = -(top + bottom) / (top - bottom);
    f32 tz = -(far + near) / (far - near);

    return Mat4 {
        Vec4(a, 0.0f, 0.0f, 0.0f),
        Vec4(0.0f, b, 0.0f, 0.0f),
        Vec4(0.0f, 0.0f, c, 0.0f),
        Vec4(tx, ty, tz, 1.0f),
    };
}

}