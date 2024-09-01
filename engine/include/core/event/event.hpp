#pragma once

#include "core/type.hpp"

namespace scsr
{

enum EventCategory : u16
{
    ECNone = 0,
    ECApplication         = BIT(0),
    ECWindow              = BIT(1),
    ECKeyboard            = BIT(2),
    ECMouseMotion         = BIT(3),
    ECMouseButton         = BIT(4),
    ECMouseWheel          = BIT(5),
    ECALL                 = 0xFFFF
};

enum class ApplicationSignal : u32
{
    Quit = 0,
};

enum class WindowSignal : u32
{
    Shown = 0,
    Hidden,
    Moved,
    Resized,
    Minimized,
    Maximized,
    Enter,
    Leave,
    FocusGained,
    FocusLost,
    Close,
};

enum class ButtonState : u32
{
    Pressed = 0,
    Released
};

/// Mouse button code
enum class MouseButton : u32
{
    Left    = 1,
    Middle  = 2,
    Right   = 3,
    X1      = 4,
    X2      = 5,
};

// Event types

/// Application and Window
struct ApplicationEvent
{
    EventCategory category = EventCategory::ECApplication;
    ApplicationSignal signal;
};

struct WindowEvent
{
    EventCategory category = EventCategory::ECWindow;
    WindowSignal signal;
    u8 windowID;
    i32 x;
    i32 y;
};

/// Keyboard
struct KeyboardEvent
{
    EventCategory category = EventCategory::ECKeyboard;
    u32 keyCode;
    ButtonState state;
    u32 repeat = 0; // repeat if non-zero
};

/// Mouse move 
struct MouseMotionEvent
{
    EventCategory category = EventCategory::ECMouseMotion;
    // logic position
    i32 x;
    i32 y;
    // relative movement
    i32 dx;
    i32 dy;
};

/// Mouse button
struct MouseButtonEvent
{
    EventCategory category = EventCategory::ECMouseButton;
    MouseButton button;
    ButtonState state;
    u32 clicks;
};

/// Mouse wheel scroll
struct MouseWheelEvent
{
    EventCategory category = EventCategory::ECMouseWheel;
    i32 x;
    i32 y;
};

struct Event
{
    union
    {
        EventCategory category;
        ApplicationEvent application;
        WindowEvent window;
        KeyboardEvent keyboard;
        MouseMotionEvent mouseMotion;
        MouseButtonEvent mouseButton;
        MouseWheelEvent mouseWheel;

        u8 _padding[20]; // force to use 20 bytes
    };

    Event() : category(EventCategory::ECNone) {}
};

}