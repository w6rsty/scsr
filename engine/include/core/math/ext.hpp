#pragma once

#include "core/assert.hpp"
#include "core/math/vector.hpp"
#include "core/type.hpp"
#include "def.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"

#include <cmath>
#include <tuple>

namespace scsr
{

inline Mat4 FromScale(const Vec3& scale)
{
    return Mat4 {
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

inline Mat4 FromRotation(const Quat& quat)
{
    return quat.ToMat4();
}

inline Mat4 FromRotation(const Vec3& euler)
{
    return Quat::FromRotationEuler(euler).ToMat4();
}

inline Mat4 FromTranslation(const Vec3& translation)
{
    return Mat4 {
        1.0f, 0.0f, 0.0f, translation.x,
        0.0f, 1.0f, 0.0f, translation.y,
        0.0f, 0.0f, 1.0f, translation.z,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

inline Mat4 FromScaleRotationTranslation(const Vec3& scale, const Quat& rotation, const Vec3& translation)
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
inline std::tuple<Vec3, Quat, Vec3> ToScaleRotationTranslation(const Mat4& mat)
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

/// Right-handed look-at matrix for object orientation
/// This is not a camera look-at matrix
inline Mat4 LookTo(const Vec3& eye, const Vec3& to, const Vec3& up)
{
    Vec3 F = Normalized(to);
    Vec3 R = Normalized(Cross(F, up));
    Vec3 U = Cross(R, F);

    return Mat4 {
        Vec4(R.x, U.x, F.x, 0.0f),
        Vec4(R.y, U.y, F.y, 0.0f),
        Vec4(R.z, U.z, F.z, 0.0f),
        Vec4(0.0f, 0.0f, 0.0f, 1.0f)
    };
}

/// Right-handed look-at matrix for camera orientation
inline Mat4 LookAt(const Vec3& eye, const Vec3& target, const Vec3& up)
{
    Vec3 F = Normalized(target - eye);
    Vec3 R = Normalized(Cross(F, up));
    Vec3 U = Normalized(Cross(R, F));

    return Mat4 {
        Vec4(R.x, U.x, -F.x, 0.0f),
        Vec4(R.y, U.y, -F.y, 0.0f),
        Vec4(R.z, U.z, -F.z, 0.0f),
        Vec4(-Dot(R, eye), -Dot(U, eye), -Dot(-F, eye), 1.0f)
    };
}

/// Right-handed OpenGL perspective projection matrix
/// Depth range from [-1, 1]
inline Mat4 ProjectionPerspectiveGL(f32 verticalFov, f32 aspectRatio, f32 near, f32 far)
{
    float recipRange = 1.0f / (near - far);
    float f = 1.0f / std::tanf(verticalFov * 0.5f);
    float a = f / aspectRatio;
    float b = (near + far) * recipRange;
    float c = 2.0f * near * far * recipRange;

    return Mat4 {
        Vec4(a, 0.0f, 0.0f, 0.0f),
        Vec4(0.0f, f, 0.0f, 0.0f),
        Vec4(0.0f, 0.0f, b, -1.0f),
        Vec4(0.0f, 0.0f, c, 0.0f)
    };
}

/// Right-handed OpenGL orthographic projection matrix
/// Depth range from [-1, 1]
inline Mat4 ProjectionOrthoGraphicGL(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
    float recipW = 1.0f / (right - left);
    float recipH = 1.0f / (top - bottom);
    float recipD = 1.0f / (near - far);
    float a = 2.0f * recipW;
    float b = 2.0f * recipH;
    float c = 2.0f * recipD;
    float tx = -(right + left) * recipW;
    float ty = -(top + bottom) * recipH;
    float tz = (near + far) * recipD;

    return Mat4 {
        Vec4(a, 0.0f, 0.0f, 0.0f),
        Vec4(0.0f, b, 0.0f, 0.0f),
        Vec4(0.0f, 0.0f, c, 0.0f),
        Vec4(tx, ty, tz, 1.0f)
    };
}

/// Right-handed OpenGL orthographic projection matrix
/// With x-y symmetry
/// Depth range from [-1, 1]
inline Mat4 ProjectionOrthoGraphicGL(f32 right, f32 top, f32 near, f32 far)
{
    float a = 1.0f / right;
    float b = 1.0f / top;
    float recipD = 1.0f / (near - far);
    float c = 2.0f * recipD;
    float tz = (near + far) * recipD;

    return Mat4 {
        Vec4(a, 0.0f, 0.0f, 0.0f),
        Vec4(0.0f, b, 0.0f, 0.0f),
        Vec4(0.0f, 0.0f, c, 0.0f),
        Vec4(0, 0, tz, 1.0f)
    };
}

/// Right-handed Perspective projection matrix
/// Depth range from [0, 1]
inline Mat4 ProjectionPerspective(f32 verticalFov, f32 aspectRatio, f32 near, f32 far)
{
    // Compute the scale factors for x and y directions
    float f = 1.0f / std::tanf(verticalFov * 0.5f); // Assumes verticalFov is in radians
    float a = f / aspectRatio;

    // Depth scaling and translation terms
    float b = far / (near - far);
    float c = near * b;

    return Mat4 {
        Vec4(a, 0.0f, 0.0f, 0.0f),
        Vec4(0.0f, f, 0.0f, 0.0f),
        Vec4(0.0f, 0.0f, b, -1.0f),
        Vec4(0.0f, 0.0f, c, 0.0f)
    };
}
 

/// Right-handed Orthographic projection matrix
/// Depth range from [0, 1]
inline Mat4 ProjectionOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
    float recipW = 1.0f / (right - left);
    float recipH = 1.0f / (top - bottom);
    float a = 2.0f * recipW;
    float b = 2.0f * recipH;
    float c = 1.0f / (near - far);
    float tx = -(right + left) * recipW;
    float ty = -(top + bottom) * recipH;
    float tz = near * c;

    return Mat4 {
        Vec4(a, 0.0f, 0.0f, 0.0f),
        Vec4(0.0f, b, 0.0f, 0.0f),
        Vec4(0.0f, 0.0f, c, 0.0f),
        Vec4(tx, ty, tz, 1.0f)
    };
}

/// Right-handed Orthographic projection matrix
/// With x-y symmetry
/// Depth range from [0, 1]
inline Mat4 ProjectionOrthographic(f32 right, f32 top, f32 near, f32 far)
{
    float a = 1.0f / right;
    float b = 1.0f / top;
    float c = 1.0f / (near - far);
    float tz = near * c;

    return Mat4 {
        Vec4(a, 0.0f, 0.0f, 0.0f),
        Vec4(0.0f, b, 0.0f, 0.0f),
        Vec4(0.0f, 0.0f, c, 0.0f),
        Vec4(0, 0, tz, 1.0f)
    };
}

using Color = Vec4;

inline Color ColorFromHex(u32 hex)
{
    return Color {
        ((hex >> 16) & 0xFF) / 255.0f,
        ((hex >> 8) & 0xFF) / 255.0f,
        (hex & 0xFF) / 255.0f,
        1.0f
    };
}

inline Color ColorFromU8(u8 r, u8 g, u8 b, u8 a = 255)
{
    return Color {
        r / 255.0f,
        g / 255.0f,
        b / 255.0f,
        a / 255.0f
    };
}

inline u32 clamp8bit(u8&& value) {
    value = value & -(value >= 0);
    value = (value | -(value > 255)) & 255;
    return std::move(value);
}

inline u32 ColorToHex(const Color& color)
{
    u32 r = clamp8bit(static_cast<u8>(color.x * 255.0f));
    u32 g = clamp8bit(static_cast<u8>(color.y * 255.0f));
    u32 b = clamp8bit(static_cast<u8>(color.z * 255.0f));
    u32 a = clamp8bit(static_cast<u8>(color.w * 255.0f));
    return (r << 24) | (g << 16) | (b << 8) | a;
}


}