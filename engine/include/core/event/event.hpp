#pragma once

#include "core/type.hpp"

namespace scsr
{

/*
| 16 bits   | 32 bits   | 16 bits   | 128 bits   |
| Category  | Type      | Flags     | Data       |
| bitmask   | index     | bitmask   | userdata   |
*/

#define EVENT_CATEGORY_MASK 0xFFFF000000000000
#define EVENT_TYPE_MASK     0x0000FFFFFFFF0000
#define EVENT_FLAGS_MASK    0x000000000000FFFF


enum class EventCategory : u16
{
    None = 0,
    Application         = BIT(0),
    Window              = BIT(1),
    Keyboard            = BIT(2),
    MouseMotion         = BIT(3),
    MouseButton         = BIT(4),
    MouseWheel          = BIT(5),
};

enum class ApplicationSignal : u32
{
    Quit = 0,
    Hidden,
    Enter,
    Leave,
    FocusGained,
    FocusLost,
};

enum class WindowSignal : u32
{
    Close = 0,
    Move,
    Resize,
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
    EventCategory category = EventCategory::Application;
    ApplicationSignal signal;
};

struct WindowEvent
{
    EventCategory category = EventCategory::Window;
    WindowSignal signal;
    i32 x;
    i32 y;
};

/// Keyboard
struct KeyboardEvent
{
    EventCategory category = EventCategory::Keyboard;
    u32 keyCode;
    ButtonState state;
    u32 repeat = 0; // repeat if non-zero
};

/// Mouse move 
struct MouseMotionEvent
{
    EventCategory category = EventCategory::MouseMotion;
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
    EventCategory category = EventCategory::MouseButton;
    MouseButton button;
    ButtonState state;
    u32 clicks;
};

/// Mouse wheel scroll
struct MouseWheelEvent
{
    EventCategory category = EventCategory::MouseWheel;
    i32 offset;
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

    Event() : category(EventCategory::None) {}
};

}