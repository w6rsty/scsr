#pragma once

#include "event/event.hpp"
#include "math/math.hpp"

#include <string>

namespace scsr
{

enum KeyboardKeyCode : i32
{
    KeyTab = 9,
    KeyEnter = 13,
    KeyEscape = 27,
    KeySpace = 32,

    Key0 = 48,
    Key1,
    Key2,
    Key3,
    Key4,
    Key5,
    Key6,
    Key7,
    Key8,
    Key9,

    KeyA = 97,
    KeyB,
    KeyC,
    KeyD,
    KeyE,
    KeyF,
    KeyG,
    KeyH,
    KeyI,
    KeyJ,
    KeyK,
    KeyL,
    KeyM,
    KeyN,
    KeyO,
    KeyP,
    KeyQ,
    KeyR,
    KeyS,
    KeyT,
    KeyU,
    KeyV,
    KeyW,
    KeyX,
    KeyY,
    KeyZ,

    KeyCapsLock = 1073741881,
    KeyLeft = 1073741904,
    KeyRight = 1073741903,
    KeyUp = 1073741906,
    KeyDown = 1073741905,

    KeyLeftControl = 1073742048,
    KeyLeftShift = 1073742049,
    KeyLeftAlt = 1073742050,
    KeySuper = 1073742051,
    KeyRightControl = 1073742052,
    KeyRightShift = 1073742053,
    KeyRightAlt = 1073742054,
};

enum MouseButtonCode : u8
{
    Left    = 1,
    Middle  = 2,
    Right   = 3,
    X1      = 4,
    X2      = 5,
};

std::string FormatKeyBoardKeyCode(i32 code);
std::string FormatMouseButtonCode(u8 code);
std::string FormatEvent(const Event& event);

constexpr std::string FormatMath(const Vec2& vec)
{
    return "Vec2: (" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")";
}
constexpr std::string FormatMath(const Vec3& vec)
{
    return "Vec3: (" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
}

constexpr std::string FormatMath(const Vec4& vec)
{
    return "Vec4: (" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ", " + std::to_string(vec.w) + ")";
}

constexpr std::string FormatMath(const Mat2& mat)
{
    std::string format = "Mat2: ";
    format += "col0 (" + std::to_string(mat.m00) + ", " + std::to_string(mat.m10) + "), "
        += "col1 (" + std::to_string(mat.m01) + ", " + std::to_string(mat.m11) + ")";
    return format;
}

constexpr std::string FormatMath(const Mat3& mat)
{
    std::string format = "Mat3: ";
    format += "col0 (" + std::to_string(mat.m00) + ", " + std::to_string(mat.m10) + ", " + std::to_string(mat.m20) + "), "
        += "col1 (" + std::to_string(mat.m01) + ", " + std::to_string(mat.m11) + ", " + std::to_string(mat.m21) + "), "
        += "col2 (" + std::to_string(mat.m02) + ", " + std::to_string(mat.m12) + ", " + std::to_string(mat.m22) + ")";
    return format;
}

constexpr std::string FormatMath(const Mat4& mat)
{
    std::string format = "Mat4: ";
    format += "col0 (" + std::to_string(mat.m00) + ", " + std::to_string(mat.m10) + ", " + std::to_string(mat.m20) + ", " + std::to_string(mat.m30) + "), "
        += "col1 (" + std::to_string(mat.m01) + ", " + std::to_string(mat.m11) + ", " + std::to_string(mat.m21) + ", " + std::to_string(mat.m31) + "), "
        += "col2 (" + std::to_string(mat.m02) + ", " + std::to_string(mat.m12) + ", " + std::to_string(mat.m22) + ", " + std::to_string(mat.m32) + "), "
        += "col3 (" + std::to_string(mat.m03) + ", " + std::to_string(mat.m13) + ", " + std::to_string(mat.m23) + ", " + std::to_string(mat.m33) + ")";
    return format;
}

constexpr std::string FormatMath(const Quat& quat)
{
    return "Quat: (w: " + std::to_string(quat.w) + " x: " + std::to_string(quat.x) + " y: " + std::to_string(quat.y) + " z: " + std::to_string(quat.z) + ")";
}

}