#pragma once

#include "def.hpp"

#include <cmath>
#include <tuple>
#include <span>

namespace scsr
{

struct Vec2
{
    union
    {
        struct { f32 x, y; };
        f32 data[2];
    };

    Vec2() = default;
    constexpr Vec2(f32 x, f32 y) : x(x), y(y) {}
    Vec2(f32* raw) { memcpy(data, raw, sizeof(f32) * 2); }

    f32 LengthSquared() const { return x * x + y * y; }
    f32 Length() const { return std::sqrt(LengthSquared()); }
    void Normalize() { *this /= Length(); }
    Vec2 Normalized() const { return *this / Length(); }
    Vec2 Abs() const { return Vec2(scsr::Abs(x), scsr::Abs(y)); }

    bool Eq(const Vec2& other) const { return scsr::Equal(x, other.x) && scsr::Equal(y, other.y); }
    bool Neq(const Vec2& other) const { return !Eq(other); }
    f32 Dot(const Vec2& other) const { return x * other.x + y * other.y; }
    f32 Cross(const Vec2& other) const { return x * other.y - y * other.x; }
    f32 Distance(const Vec2& other) const { return (*this - other).Length(); }
    f32 DistanceSquared(const Vec2& other) const { return (*this - other).LengthSquared(); }
    f32 ElementSum() const { return x + y; }
    f32 ElementProduct() const { return x * y; }
    f32 ElementMax() const { return scsr::Max(x, y); }
    f32 ElementMin() const { return scsr::Min(x, y); }
    bool IsFinite() const { return scsr::IsFinite(x) && scsr::IsFinite(y); }
    bool IsZero() const { return scsr::Equal(x, 0.0f) && scsr::Equal(y, 0.0f); }
    bool IsIdentity() const { return scsr::Equal(x, 1.0f) && scsr::Equal(y, 1.0f); }
    bool IsNormalized() const { return scsr::Equal(LengthSquared(), 1.0f); }

    Vec2 operator + (const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator - (const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator * (f32 scalar) const { return Vec2(x * scalar, y * scalar); }
    Vec2 operator / (f32 scalar) const { return Vec2(x / scalar, y / scalar); }
    Vec2 operator - () const { return Vec2(-x, -y); }
    Vec2& operator += (const Vec2& other) { x += other.x; y += other.y; return *this; }
    Vec2& operator -= (const Vec2& other) { x -= other.x; y -= other.y; return *this; }
    Vec2& operator *= (f32 scalar) { x *= scalar; y *= scalar; return *this; }
    Vec2& operator /= (f32 scalar) { x /= scalar; y /= scalar; return *this; }
    
    static constexpr Vec2 Splat(f32 v) { return Vec2(v, v); }

    static constexpr Vec2 X() { return Vec2(1.0f, 0.0f); }
    static constexpr Vec2 Y() { return Vec2(0.0f, 1.0f); }
    static constexpr Vec2 ZERO() { return Splat(0.0f); }
    static constexpr Vec2 ONE() { return Splat(1.0f); }
    static constexpr Vec2 NEG_X() { return Vec2(-1.0f, 0.0f); }
    static constexpr Vec2 NEG_Y() { return Vec2(0.0f, -1.0f); }
    static constexpr Vec2 NEG_ONE() { return Vec2(-1.0f, -1.0f); }
    static constexpr Vec2 MIN() { return Splat(F32MIN); }
    static constexpr Vec2 MAX() { return Splat(F32MAX); }
    static constexpr Vec2 INF() { return Splat(F32INF); }
    /// Not a number
    static constexpr Vec2 NANM() { return Splat(F32NAN); }

    // group of aixs
    static constexpr std::tuple<Vec2, Vec2> AXES() { return std::make_tuple(X(), Y()); }
};


inline f32 Length(const Vec2& v) { return v.Length(); }
inline f32 LengthSquared(const Vec2& v) { return v.LengthSquared(); }
inline Vec2 Normalized(const Vec2& v) { return v.Normalized(); }
inline Vec2 Abs(const Vec2& v) { return v.Abs(); }
inline bool Equal(const Vec2& a, const Vec2& b) { return a.Eq(b); }
inline bool NotEqual(const Vec2& a, const Vec2& b) { return a.Neq(b); }
inline Vec2 Max(const Vec2& a, const Vec2& b) { return Vec2(scsr::Max(a.x, b.x), scsr::Max(a.y, b.y)); }
inline Vec2 Min(const Vec2& a, const Vec2& b) { return Vec2(scsr::Min(a.x, b.x), scsr::Min(a.y, b.y)); }
inline Vec2 Clamp(const Vec2& value, const Vec2& min, const Vec2& max) { return Vec2(
    scsr::Clamp(value.x, min.x, max.x),
    scsr::Clamp(value.y, min.y, max.y)); }
inline f32 Dot(const Vec2& a, const Vec2& b) { return a.Dot(b); }
inline f32 Cross(const Vec2& a, const Vec2& b) { return a.Cross(b); }
inline Vec2 Lerp(const Vec2& a, const Vec2& b, f32 t) { return a + (b - a) * t; }

struct Vec3
{
    union
    {
        struct { f32 x, y, z; };
        f32 data[3];
    };

    Vec3() = default;
    constexpr Vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    Vec3(f32* raw) { memcpy(data, raw, sizeof(f32) * 3); }

    Vec3(const Vec2& v2, f32 z) : x(v2.x), y(v2.y), z(z) {}
    Vec3(f32 x, const Vec2& v2) : x(x), y(v2.x), z(v2.y) {}

    std::span<f32> xy() { return std::span<f32>(data, 2); }
    std::span<f32> yz() { return std::span<f32>(data + 1, 2); }

    f32 LengthSquared() const { return x * x + y * y + z * z; }
    f32 Length() const { return std::sqrt(LengthSquared()); }
    void Normalize() { *this /= Length(); }
    Vec3 Normalized() const { return *this / Length(); }
    Vec3 Abs() const { return Vec3(scsr::Abs(x), scsr::Abs(y), scsr::Abs(z)); }

    bool Eq(const Vec3& other) const { return scsr::Equal(x, other.x) && scsr::Equal(y, other.y) && scsr::Equal(z, other.z); }
    bool Neq(const Vec3& other) const { return !Eq(other); }
    f32 Dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }
    Vec3 Cross(const Vec3& other) const { return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }
    f32 Distance(const Vec3& other) const { return (*this - other).Length(); }
    f32 DistanceSquared(const Vec3& other) const { return (*this - other).LengthSquared(); }
    f32 ElementSum() const { return x + y + z; }
    f32 ElementProduct() const { return x * y * z; }
    f32 ElementMax() const { return scsr::Max(x, scsr::Max(y, z)); }
    f32 ElementMin() const { return scsr::Min(x, scsr::Min(y, z)); }
    bool IsFinite() const { return scsr::IsFinite(x) && scsr::IsFinite(y) && scsr::IsFinite(z); }
    bool IsZero() const { return scsr::Equal(x, 0.0f) && scsr::Equal(y, 0.0f) && scsr::Equal(z, 0.0f); }
    bool IsIdentity() const { return scsr::Equal(x, 1.0f) && scsr::Equal(y, 1.0f) && scsr::Equal(z, 1.0f); }
    bool IsNormalized() const { return scsr::Equal(LengthSquared(), 1.0f); }

    Vec3 operator + (const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator - (const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator * (f32 scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
    Vec3 operator / (f32 scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }
    Vec3 operator - () const { return Vec3(-x, -y, -z); }
    Vec3& operator += (const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vec3& operator -= (const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Vec3& operator *= (f32 scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vec3& operator /= (f32 scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    static constexpr Vec3 Splat(f32 v) { return Vec3(v, v, v); }

    static constexpr Vec3 X() { return Vec3(1.0f, 0.0f, 0.0f); }
    static constexpr Vec3 Y() { return Vec3(0.0f, 1.0f, 0.0f); }
    static constexpr Vec3 Z() { return Vec3(0.0f, 0.0f, 1.0f); }
    static constexpr Vec3 ZERO() { return Vec3(0.0f, 0.0f, 0.0f); }
    static constexpr Vec3 ONE() { return Vec3(1.0f, 1.0f, 1.0f); }
    static constexpr Vec3 NEG_X() { return Vec3(-1.0f, 0.0f, 0.0f); }
    static constexpr Vec3 NEG_Y() { return Vec3(0.0f, -1.0f, 0.0f); }
    static constexpr Vec3 NEG_Z() { return Vec3(0.0f, 0.0f, -1.0f); }
    static constexpr Vec3 NEG_ONE() { return Vec3(-1.0f, -1.0f, -1.0f); }
    static constexpr Vec3 MIN() { return Splat(F32MIN); }
    static constexpr Vec3 MAX() { return Splat(F32MAX); }
    static constexpr Vec3 INF() { return Splat(F32INF); }
    /// Not a number
    static constexpr Vec3 NANM() { return Splat(F32NAN); }

    // group of aixs
    static constexpr std::tuple<Vec3, Vec3, Vec3> AXES() { return std::make_tuple(X(), Y(), Z()); }
};

inline f32 Length(const Vec3& v) { return v.Length(); }
inline f32 LengthSquared(const Vec3& v) { return v.LengthSquared(); }
inline Vec3 Normalized(const Vec3& v) { return v.Normalized(); }
inline Vec3 Abs(const Vec3& v) { return v.Abs(); }
inline bool Equal(const Vec3& a, const Vec3& b) { return a.Eq(b); }
inline bool NotEqual(const Vec3& a, const Vec3& b) { return a.Neq(b); }
inline Vec3 Max(const Vec3& a, const Vec3& b) { return Vec3(scsr::Max(a.x, b.x), scsr::Max(a.y, b.y), scsr::Max(a.z, b.z)); }
inline Vec3 Min(const Vec3& a, const Vec3& b) { return Vec3(scsr::Min(a.x, b.x), scsr::Min(a.y, b.y), scsr::Min(a.z, b.z)); }
inline Vec3 Clamp(const Vec3& value, const Vec3& min, const Vec3& max) { return Vec3(
    scsr::Clamp(value.x, min.x, max.x),
    scsr::Clamp(value.y, min.y, max.y),
    scsr::Clamp(value.z, min.z, max.z)); }
inline f32 Dot(const Vec3& a, const Vec3& b) { return a.Dot(b); }
inline Vec3 Cross(const Vec3& a, const Vec3& b) { return a.Cross(b); }
inline Vec3 Lerp(const Vec3& a, const Vec3& b, f32 t) { return a + (b - a) * t; }

struct Vec4
{
    union
    {
        struct { f32 x, y, z, w; };
        f32 data[4];
    };

    Vec4() = default;
    constexpr Vec4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
    Vec4(f32* raw) { memcpy(data, raw, sizeof(f32) * 4); }

    Vec4(const Vec2& v, const Vec2& u) : x(v.x), y(v.y), z(u.x), w(u.y) {}
    Vec4(const Vec3& v3, f32 w) : x(v3.x), y(v3.y), z(v3.z), w(w) {}

    Vec3 Trancate() const { return Vec3(x, y, z); }
    std::span<f32> xyz() { return std::span<f32>(data, 3); }
    std::span<f32> xy() { return std::span<f32>(data, 2); }
    std::span<f32> zw() { return std::span<f32>(data + 2, 2); }
    
    f32 LengthSquared() const { return x * x + y * y + z * z + w * w; }
    f32 Length() const { return std::sqrt(LengthSquared()); }
    void Normalize() { *this /= Length(); }
    Vec4 Abs() const { return Vec4(scsr::Abs(x), scsr::Abs(y), scsr::Abs(z), scsr::Abs(w)); }

    bool Eq(const Vec4& other) const { return scsr::Equal(x, other.x) && scsr::Equal(y, other.y) && scsr::Equal(z, other.z) && scsr::Equal(w, other.w); }
    bool Neq(const Vec4& other) const { return !Eq(other); }
    f32 ElementSum() const { return x + y + z + w; }
    f32 ElementProduct() const { return x * y * z * w; }
    f32 ElementMax() const { return scsr::Max(x, scsr::Max(y, scsr::Max(z, w))); }
    f32 ElementMin() const { return scsr::Min(x, scsr::Min(y, scsr::Min(z, w))); }
    bool IsFinite() const { return scsr::IsFinite(x) && scsr::IsFinite(y) && scsr::IsFinite(z) && scsr::IsFinite(w); }
    bool IsZero() const { return scsr::Equal(x, 0.0f) && scsr::Equal(y, 0.0f) && scsr::Equal(z, 0.0f) && scsr::Equal(w, 0.0f); }
    bool IsIdentity() const { return scsr::Equal(x, 1.0f) && scsr::Equal(y, 1.0f) && scsr::Equal(z, 1.0f) && scsr::Equal(w, 1.0f); }
    bool IsNormalized() const { return scsr::Equal(LengthSquared(), 1.0f); }
    
    Vec4 operator + (const Vec4& other) const { return Vec4(x + other.x, y + other.y, z + other.z, w + other.w); }
    Vec4 operator - (const Vec4& other) const { return Vec4(x - other.x, y - other.y, z - other.z, w - other.w); }
    Vec4 operator * (f32 scalar) const { return Vec4(x * scalar, y * scalar, z * scalar, w * scalar); }
    Vec4 operator / (f32 scalar) const { return Vec4(x / scalar, y / scalar, z / scalar, w / scalar); }
    Vec4 operator - () const { return Vec4(-x, -y, -z, -w); }
    Vec4& operator += (const Vec4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
    Vec4& operator -= (const Vec4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
    Vec4& operator *= (f32 scalar) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
    Vec4& operator /= (f32 scalar) { x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }

    static constexpr Vec4 Splat(f32 v) { return Vec4(v, v, v, v); }

    static constexpr Vec4 X() { return Vec4(1.0f, 0.0f, 0.0f, 0.0f); }
    static constexpr Vec4 Y() { return Vec4(0.0f, 1.0f, 0.0f, 0.0f); }
    static constexpr Vec4 Z() { return Vec4(0.0f, 0.0f, 1.0f, 0.0f); }
    static constexpr Vec4 W() { return Vec4(0.0f, 0.0f, 0.0f, 1.0f); }
    static constexpr Vec4 ZERO() { return Vec4(0.0f, 0.0f, 0.0f, 0.0f); }
    static constexpr Vec4 ONE() { return Vec4(1.0f, 1.0f, 1.0f, 1.0f); }
    static constexpr Vec4 NEG_X() { return Vec4(-1.0f, 0.0f, 0.0f, 0.0f); }
    static constexpr Vec4 NEG_Y() { return Vec4(0.0f, -1.0f, 0.0f, 0.0f); }
    static constexpr Vec4 NEG_Z() { return Vec4(0.0f, 0.0f, -1.0f, 0.0f); }
    static constexpr Vec4 NEG_W() { return Vec4(0.0f, 0.0f, 0.0f, -1.0f); }
    static constexpr Vec4 NEG_ONE() { return Vec4(-1.0f, -1.0f, -1.0f, -1.0f); }
    static constexpr Vec4 MIN() { return Splat(F32MIN); }
    static constexpr Vec4 MAX() { return Splat(F32MAX); }
    static constexpr Vec4 INF() { return Splat(F32INF); }
    /// Not a number
    static constexpr Vec4 NANM() { return Splat(F32NAN); }

    // group of aixs
    static constexpr std::tuple<Vec4, Vec4, Vec4, Vec4> AXES() { return std::make_tuple(X(), Y(), Z(), W()); }
};

inline Vec4 Abs(const Vec4& v) { return v.Abs(); }
inline bool Equal(const Vec4& a, const Vec4& b) { return a.Eq(b); }
inline bool NotEqual(const Vec4& a, const Vec4& b) { return a.Neq(b); }

}