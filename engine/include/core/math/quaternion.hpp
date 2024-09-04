#pragma once

#include "core/assert.hpp"
#include "def.hpp"
#include "matrix.hpp"
#include "vector.hpp"

namespace scsr
{

struct Quat
{
    union 
    {
        struct { f32 w, x, y, z; };
        struct {
            f32 s;
            Vec3 v3;
        };
        Vec4 v4; // to simplify internal operation
    };

    constexpr Quat(f32 w, f32 x, f32 y, f32 z) : w(w), x(x), y(y), z(z) {}
    constexpr Quat(f32 s, const Vec3& v) : s(s), v3(v) {}
    /// Notice the order of the elements is different from Vec4 
    constexpr Quat(const Vec4& v) : v4(v) {}
    Quat(float* raw) : v4(raw) {}

    f32 Mangitude() const { return v4.Length(); }
    f32 MangitudeSquared() const { return v4.LengthSquared(); }
    void Normalize() { v4.Normalize(); }
    [[nodiscard]] Quat Normalized() const
    {
        Quat ret = *this;
        ret.Normalize();
        return ret;
    }
    void Conjugate() { v3 = -v3; }
    [[nodiscard]] Quat Conjugated() const
    {
        Quat ret = *this;
        ret.Conjugate();
        return ret;
    }
    void Inverse()
    {
        Conjugate();
        v4 /= MangitudeSquared();
    }
    [[nodiscard]] Quat Inversed() const
    {
        Quat ret = *this;
        ret.Inverse();
        return ret;
    }
    bool IsZero() const { return v4.IsZero(); }
    bool IsIdentity() const { return Equal(s, 1.0f) && v3.IsZero(); }
    bool IsNormalized() const { return Equal(MangitudeSquared(), 1.0f); }

    f32 Scalar() const { return s; }
    Vec3 Vector() const { return v3; }

    Quat operator * (const Quat& rhs)
    {
        return Quat {
            w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
            w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
            w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
            w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w
        };
    }
    Quat& operator *= (const Quat& rhs)
    {
        *this = *this * rhs;
        return *this;
    }
    // Convert to 3x3 rotation matrix
    Mat3 ToMat3() const
    {
        f32 x2 = x + x;
        f32 y2 = y + y;
        f32 z2 = z + z;
        f32 xx = x * x2;
        f32 xy = x * y2;
        f32 xz = x * z2;
        f32 yy = y * y2;
        f32 yz = y * z2;
        f32 zz = z * z2;
        f32 wx = w * x2;
        f32 wy = w * y2;
        f32 wz = w * z2;

        return Mat3 {
            Vec3(1.0 - (yy + zz), xy + wz, xz - wy),
            Vec3(xy - wz, 1.0 - (xx + zz), yz + wx),
            Vec3(xz + wy, yz - wx, 1.0 - (xx + yy)),
        };
    }
    /// Convert to 4x4 rotation matrix
    Mat4 ToMat4() const { return Mat4 { ToMat3() }; }

    Quat operator + (const Quat& rhs) const { return v4 + rhs.v4; }
    Quat operator - (const Quat& rhs) const { return v4 - rhs.v4; }
    Quat operator * (f32 rhs) const { return v4 * rhs; }
    Quat operator / (f32 rhs) const { return v4 / rhs; }
    Quat operator - () const { return Quat(-v4); }
    Quat& operator += (const Quat& rhs) { v4 += rhs.v4; return *this; }
    Quat& operator -= (const Quat& rhs) { v4 -= rhs.v4; return *this; }
    Quat& operator *= (f32 rhs) { v4 *= rhs; return *this; }
    Quat& operator /= (f32 rhs) { v4 /= rhs; return *this; }

    /// Rotate around axis in radians
    static Quat FromAxisAngle(const Vec3& axis, f32 angle)
    {
        f32 halfAngle = angle * 0.5f;
        return Quat(std::cos(halfAngle), axis * std::sin(halfAngle));
    }
    /// Rotate around X in radians
    static Quat FromRotationX(f32 angle) { return FromAxisAngle(Vec3::X(), angle); }
    /// Rotate around Y in radians
    static Quat FromRotationY(f32 angle) { return FromAxisAngle(Vec3::Y(), angle); }
    /// Rotate around Z in radians
    static Quat FromRotationZ(f32 angle) { return FromAxisAngle(Vec3::Z(), angle); }
    /// Rotate in euler angles in radians
    static Quat FromRotationEuler(const Vec3& euler)
    {
        Quat qX = FromRotationX(euler.x);
        Quat qY = FromRotationY(euler.y);
        Quat qZ = FromRotationZ(euler.z);
        return qZ * qY * qX;
    }
    /// From 3x3 rotation matrix
    static Quat FromMat3(const Mat3& mat)
    {
        /// Copy from glam-0.27.0/src/f32/sse2/quat.rs
        auto [m00, m01, m02] = mat.col0.data;
        auto [m10, m11, m12] = mat.col1.data;
        auto [m20, m21, m22] = mat.col2.data;
        if (m22 <= 0.0f)
        {
            f32 dif10 = m11 - m00;
            f32 omm22 = 1.0f - m22;
            if (dif10 <= 0.0f)
            {
                f32 four_xsq = omm22 - dif10;
                f32 inv4x = 0.5f / std::sqrt(four_xsq);
                return Quat {
                    (m12 - m21) * inv4x,
                    four_xsq * inv4x,
                    (m01 + m10) * inv4x,
                    (m02 + m20) * inv4x
                };
            }
            else
            {
                f32 four_ysq = omm22 + dif10;
                f32 inv4y = 0.5f / std::sqrt(four_ysq);
                return Quat {
                    (m20 - m02) * inv4y,
                    (m01 + m10) * inv4y,
                    four_ysq * inv4y,
                    (m12 + m21) * inv4y,
                };
            }
        }
        else
        {
            f32 sum10 = m11 + m00;
            f32 opm22 = 1.0f + m22;
            if (sum10 <= 0.0f)
            {
                f32 four_zsq = opm22 - sum10;
                f32 inv4z = 0.5f / std::sqrt(four_zsq);
                return Quat {
                    (m01 - m10) * inv4z,
                    (m02 + m20) * inv4z,
                    (m12 + m21) * inv4z,
                    four_zsq * inv4z,
                };
            }
            else
            {
                f32 four_wsq = opm22 + sum10;
                f32 inv4w = 0.5f / std::sqrt(four_wsq);
                return Quat {
                    four_wsq * inv4w,
                    (m12 - m21) * inv4w,
                    (m20 - m02) * inv4w,
                    (m01 - m10) * inv4w,
                };
            }
        }
    }
    /// From 4x4 rotation matrix
    static Quat FromMat4(const Mat4& mat) { return FromMat3(mat.ToMat3()); }

    static constexpr Quat IDENTITY() { return Quat(1.0f, 0.0f, 0.0f, 0.0f); }
    static constexpr Quat ZERO() { return Quat(0.0f, 0.0f, 0.0f, 0.0f); }
};

inline void RotationAxisAngle(Vec3& v, const Vec3& axis, f32 angle)
{
    Quat q = Quat::FromAxisAngle(axis, angle);
    Quat result = q * Quat(0.0f, v) * q.Conjugated();
    v = result.Vector();
}
inline void RotationX(Vec3& v, f32 angle) { RotationAxisAngle(v, Vec3::X(), angle); }
inline void RotationY(Vec3& v, f32 angle) { RotationAxisAngle(v, Vec3::Y(), angle); }
inline void RotationZ(Vec3& v, f32 angle) { RotationAxisAngle(v, Vec3::Z(), angle); }
inline void RotationEuler(Vec3& v, const Vec3& euler)
{
    Quat q = Quat::FromRotationEuler(euler);
    Quat result = q * Quat(0.0f, v) * q.Conjugated();
    v = result.Vector();
}

/// Linear interpolation
inline Quat Lerp(const Quat& q0, const Quat& q1, f32 t)
{
    return q0 * (1.0f - t) + q1 * t;
}

/// Normalized linear interpolation
inline Quat Nlerp(const Quat& q0, const Quat& q1, f32 t)
{
    Quat q = Lerp(q0, q1, t);
    q.Normalize();
    return q;
}

/// Spherical linear interpolation
/// Slerp will fallback to Nlerp two quaternions are close enough
inline Quat Slerp(const Quat& q0, const Quat& q1, f32 t)
{
    RT_ASSERT(!q0.IsNormalized(), "Quat q0 not normalied");
    RT_ASSERT(!q1.IsNormalized(), "Quat q1 not normalied");

    const f32 threshold = 0.9995f;
    f32 dot = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
    if (scsr::Abs(dot) > threshold)
    {
        return Nlerp(q0, q1, t);
    }
    f32 angle = std::acos(dot);
    return (q0 * std::sin(angle * (1.0f - t)) + q1 * std::sin(angle * t)) / std::sin(angle);
}

}