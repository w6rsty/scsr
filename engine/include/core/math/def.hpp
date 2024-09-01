#pragma once

#include "core/type.hpp" // IWYU pragma: keep

#include <limits>
#include <cmath>

namespace scsr
{

constexpr float F32MIN = std::numeric_limits<float>::min();
constexpr float F32MAX = std::numeric_limits<float>::max();
constexpr float F32INF = std::numeric_limits<float>::infinity();
constexpr float F32NAN = std::numeric_limits<float>::quiet_NaN();
constexpr float F32EPSILON = std::numeric_limits<float>::epsilon();

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
bool IsFinite(T value) { return std::isfinite(value); }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Abs(T value) { return value < 0 ? -value : value; }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline bool Equal(T a, T b) { return std::abs(a - b) < std::numeric_limits<T>::epsilon(); }

/// Liner interpolation
template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Lerp(T a, T b, T t) { return a + (b - a) * t; }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Max(T a, T b) { return a = a > b ? a : b; }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Min(T a, T b) { return a = a < b ? a : b; }

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Clamp(T value, T min, T max) { return value < min ? min : value > max ? max : value; }


}