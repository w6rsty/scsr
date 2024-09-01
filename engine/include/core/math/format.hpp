#pragma once

#include "math.hpp"

#include <string>

namespace scsr
{

constexpr std::string FormatMath(const Vec2& vec)
{
    return "Vec2:\t{ x: " + std::to_string(vec.x) + " y: " + std::to_string(vec.y) + " }";
}
constexpr std::string FormatMath(const Vec3& vec)
{
    return "Vec3:\t{ x: " + std::to_string(vec.x) + " y: " + std::to_string(vec.y) + " z: " + std::to_string(vec.z) + " }";
}

constexpr std::string FormatMath(const Vec4& vec)
{
    return "Vec4:\t{ x: " + std::to_string(vec.x) + " y: " + std::to_string(vec.y) + " z: " + std::to_string(vec.z) + " w: " + std::to_string(vec.w) + " }";
}

constexpr std::string FormatMath(const Mat2& mat)
{
    std::string format = "Mat2:";
    format += "\t{ " + std::to_string(mat.m00) + ", " + std::to_string(mat.m01) + " }\n";
    format += "\t{ " + std::to_string(mat.m10) + ", " + std::to_string(mat.m11) + " }";
    return format;
}

constexpr std::string FormatMath(const Mat3& mat)
{
    std::string format = "Mat3:";
    format += "\t{ " + std::to_string(mat.m00) + ", " + std::to_string(mat.m01) + ", " + std::to_string(mat.m02) + " }\n";
    format += "\t{ " + std::to_string(mat.m10) + ", " + std::to_string(mat.m11) + ", " + std::to_string(mat.m12) + " }\n";
    format += "\t{ " + std::to_string(mat.m20) + ", " + std::to_string(mat.m21) + ", " + std::to_string(mat.m22) + " }";
    return format;
}

constexpr std::string FormatMath(const Mat4& mat)
{
    std::string format = "Mat4:";
    format += "\t{ " + std::to_string(mat.m00) + ", " + std::to_string(mat.m01) + ", " + std::to_string(mat.m02) + ", " + std::to_string(mat.m03) + " }\n";
    format += "\t{ " + std::to_string(mat.m10) + ", " + std::to_string(mat.m11) + ", " + std::to_string(mat.m12) + ", " + std::to_string(mat.m13) + " }\n";
    format += "\t{ " + std::to_string(mat.m20) + ", " + std::to_string(mat.m21) + ", " + std::to_string(mat.m22) + ", " + std::to_string(mat.m23) + " }\n";
    format += "\t{ " + std::to_string(mat.m30) + ", " + std::to_string(mat.m31) + ", " + std::to_string(mat.m32) + ", " + std::to_string(mat.m33) + " }";
    return format;
}

}