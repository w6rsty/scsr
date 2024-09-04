#pragma once

#include "core/type.hpp"

namespace scsr
{

enum EventCategory : u16
{
    ECNone = 0,
    ECApp         = BIT(0),
    ECWindow              = BIT(1),
    ECKeyboard            = BIT(2),
    ECMouseMotion         = BIT(3),
    ECMouseButton         = BIT(4),
    ECMouseWheel          = BIT(5),
    ECALL                 = 0xFFFF
};

enum class EventType
{
    None = 0, 
    AppExit,
    WindowShown,
    WindowMoved,
    WindowResized,
    WindowMinimized,
    WindowMaximized,
    WindowEnter,
    WindowLeave,
    WindowFocusGained,
    WindowFocusLost,
    WindowClose,
    KeyboardPressed,
    KeyboardReleased,
    MouseMotion,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseWheel
};

struct AppExitEvent
{  
    EventCategory category = EventCategory::ECApp;
    EventType type = EventType::AppExit;

    static EventType Type() { return EventType::AppExit; }
};

struct WindowShowEvent
{
    EventCategory category = EventCategory::ECWindow;
    EventType type = EventType::WindowShown;
    u32 windowID;

    static EventType Type() { return EventType::WindowShown; }
};

struct WindowMoveEvent
{
    EventCategory category = EventCategory::ECWindow;
    EventType type = EventType::WindowMoved;
    u32 windowID;
    i32 x;
    i32 y;

    static EventType Type() { return EventType::WindowMoved; }
};

struct WindowResizeEvent
{
    EventCategory category = EventCategory::ECWindow;
    EventType type = EventType::WindowResized;
    u32 windowID;
    i32 x;
    i32 y;

    static EventType Type() { return EventType::WindowResized; }
};

struct WindowMinimizeEvent
{
    EventCategory category = EventCategory::ECWindow;
    EventType type = EventType::WindowMinimized;
    u32 windowID;

    static EventType Type() { return EventType::WindowMinimized; }
};

struct WindowMaximizeEvent
{
    EventCategory category = EventCategory::ECWindow;
    EventType type = EventType::WindowMaximized;
    u32 windowID;

    static EventType Type() { return EventType::WindowMaximized; }
};

struct WindowEnterEvent
{
    EventCategory category = EventCategory::ECWindow;
    EventType type = EventType::WindowEnter;
    u32 windowID;

    static EventType Type() { return EventType::WindowEnter; }
};

struct WindowLeaveEvent
{
    EventCategory category = EventCategory::ECWindow;
    EventType type = EventType::WindowLeave;
    u32 windowID;

    static EventType Type() { return EventType::WindowLeave; }
};

struct WindowFocusGainedEvent
{
    EventCategory category = EventCategory::ECWindow;
    EventType type = EventType::WindowFocusGained;
    u32 windowID;

    static EventType Type() { return EventType::WindowFocusGained; }
};

struct WindowFocusLostEvent
{
    EventCategory category = EventCategory::ECWindow;
    EventType type = EventType::WindowFocusLost;
    u32 windowID;

    static EventType Type() { return EventType::WindowFocusLost; }
};

struct WindowCloseEvent
{
    EventCategory category = EventCategory::ECWindow;
    EventType type = EventType::WindowClose;
    u32 windowID;

    static EventType Type() { return EventType::WindowClose; }
};

struct KeyboardPressedEvent
{
    EventCategory category = EventCategory::ECKeyboard;
    EventType type = EventType::KeyboardPressed;
    i32 keyCode;
    u32 repeat = 0; // repeat if non-zero

    static EventType Type() { return EventType::KeyboardPressed; }
};

struct KeyboardReleasedEvent
{
    EventCategory category = EventCategory::ECKeyboard;
    EventType type = EventType::KeyboardReleased;
    i32 keyCode;
    u32 repeat = 0; // repeat if non-zero

    static EventType Type() { return EventType::KeyboardReleased; }
};

struct MouseMotionEvent
{
    EventCategory category = EventCategory::ECMouseMotion;
    EventType type = EventType::MouseMotion;
    i32 x;
    i32 y;
    i32 dx;
    i32 dy;

    static EventType Type() { return EventType::MouseMotion; }
};

struct MouseButtonPressedEvent
{
    EventCategory category = EventCategory::ECMouseButton;
    EventType type = EventType::MouseButtonPressed;
    u8 button;
    u32 clicks;
    i32 cx;
    i32 cy;

    static EventType Type() { return EventType::MouseButtonPressed; }
};

struct MouseButtonReleasedEvent
{
    EventCategory category = EventCategory::ECMouseButton;
    EventType type = EventType::MouseButtonReleased;
    u8 button;
    u32 clicks;
    i32 cx;
    i32 cy;

    static EventType Type() { return EventType::MouseButtonReleased; }
};

struct MouseWheelEvent
{
    EventCategory category = EventCategory::ECMouseWheel;
    EventType type = EventType::MouseWheel;
    i32 x;
    i32 y;
    i32 cx;
    i32 cy;

    static EventType Type() { return EventType::MouseWheel; }
};

struct Event
{
    union
    {
        struct
        {
            EventCategory category;
            EventType type;
        };

        AppExitEvent applicationQuit;
        
        WindowShowEvent windowShow;
        WindowMoveEvent windowMove;
        WindowResizeEvent windowResize;
        WindowMinimizeEvent windowMinimize;
        WindowMaximizeEvent windowMaximize;
        WindowEnterEvent windowEnter;
        WindowLeaveEvent windowLeave;
        WindowFocusGainedEvent windowFocusGained;
        WindowFocusLostEvent windowFocusLost;
        WindowCloseEvent windowClose;

        KeyboardPressedEvent keyboardPressed;
        KeyboardReleasedEvent keyboardReleased;

        MouseMotionEvent mouseMotion;
        MouseButtonPressedEvent mouseButtonPressed;
        MouseButtonReleasedEvent mouseButtonReleased;
        MouseWheelEvent mouseWheel;
    };

    Event() : category(EventCategory::ECNone), type(EventType::None) {}
};

}