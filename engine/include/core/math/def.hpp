#pragma once

#include "core/type.hpp"

#include <limits>
#include <cmath>

namespace scsr
{

constexpr f32 F32MIN = std::numeric_limits<f32>::min();
constexpr f32 F32MAX = std::numeric_limits<f32>::max();
constexpr f32 F32INF = std::numeric_limits<f32>::infinity();
constexpr f32 F32NAN = std::numeric_limits<f32>::quiet_NaN();
constexpr f32 F32EPSILON = std::numeric_limits<f32>::epsilon();

constexpr f32 PI = 3.14159265359f;

constexpr f32 Radians(f32 degrees) { return degrees * PI / 180.0f; }
constexpr f32 Degrees(f32 radians) { return radians * 180.0f / PI; }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
bool IsFinite(const T& value) { return std::isfinite(value); }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Abs(const T& value) { return value < 0 ? -value : value; }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline bool Equal(const T& a, const T& b) { return std::abs(a - b) < std::numeric_limits<T>::epsilon(); }

/// Liner interpolation
template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Lerp(const T& a, const T b, T& t) { return a + (b - a) * t; }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Max(const T a, const T& b) { return a > b ? a : b; }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Min(const T& a, const T& b) { return a < b ? a : b; }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Clamp(const T& value, const T& min, const T& max) { return value < min ? min : value > max ? max : value; }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Signum(const T& value) { return value < 0 ? -1 : value > 0 ? 1 : 0; }

/* Forward declaration */
struct Vec2;
struct Vec3;
struct Vec4;
struct Mat2;
struct Mat3;
struct Mat4;
struct Quat;

}