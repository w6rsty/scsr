#pragma once

#include "core/type.hpp"

#include <string>

namespace scsr
{

/*
| 8 bits    | 20 bits   | 4 bits    | 32 bits   |
| Category  | Type      | Flags     | Data      |
| bitmask   | index     | bitmask   | userdata  |
*/

#define EVENT_CATEGORY_MASK 0xFF00000000000000
#define EVENT_TYPE_MASK     0x00FFFFF000000000
#define EVENT_FLAGS_MASK    0x0000000F00000000
#define EVENT_DATA_MASK     0x00000000FFFFFFFF

#define MAKE_EVENT_TYPE(category, index) ((u32)category << 24) | ((u32)BIT(index) << 20)

enum class EventCategory
{
    None = 0,
    Application = BIT(0),
    Keyboard    = BIT(1),
    Mouse       = BIT(2),
    MouseButton = BIT(3),
};

enum class EventType : u32
{
    None = 0,
    // Application
    WindowClose         = MAKE_EVENT_TYPE(EventCategory::Application, 0),
    WindowResize        = MAKE_EVENT_TYPE(EventCategory::Application, 1),
    // Keyboard
    KeyPressed          = MAKE_EVENT_TYPE(EventCategory::Keyboard, 0),
    KeyReleased         = MAKE_EVENT_TYPE(EventCategory::Keyboard, 1),
    // Mouse
    MouseMoved          = MAKE_EVENT_TYPE(EventCategory::Mouse, 0),
    MouseScrolled       = MAKE_EVENT_TYPE(EventCategory::Mouse, 1),
    // MouseButton
    MouseButtonPressed  = MAKE_EVENT_TYPE(EventCategory::MouseButton, 0),
    MouseButtonReleased = MAKE_EVENT_TYPE(EventCategory::MouseButton, 1),
};

struct Event
{
    u32 type;
    u32 data;

    Event() = default;
    Event(EventType type, unsigned char flags, u32 data) :
        type(((u32)type << 4) | (u32)flags),
        data(data)
    {}

    EventCategory GetCategory() const { return (EventCategory)(type & EVENT_CATEGORY_MASK >> 56); }
    EventType GetType() const { return (EventType)(type & EVENT_TYPE_MASK >> 32); }
    /// No flags for now

    bool IsInCategory(EventCategory category) const
    {
        return (u8)GetCategory() & (u8)category;
    }
    
};

static std::string EventCategoryToString(EventCategory category)
{
    switch (category)
    {
        case EventCategory::Application:    return "Application";
        case EventCategory::Keyboard:       return "Keyboard";
        case EventCategory::Mouse:          return "Mouse";
        case EventCategory::MouseButton:    return "MouseButton";
        default:                            return "Unknown";
    }
}

static std::string EventTypeToString(EventType type)
{
    switch (type)
    {
        case EventType::WindowClose:            return "WindowClose";
        case EventType::WindowResize:           return "WindowResize";
        case EventType::KeyPressed:             return "KeyPressed";
        case EventType::KeyReleased:            return "KeyReleased";
        case EventType::MouseMoved:             return "MouseMoved";
        case EventType::MouseScrolled:          return "MouseScrolled";
        case EventType::MouseButtonPressed:     return "MouseButtonPressed";
        case EventType::MouseButtonReleased:    return "MouseButtonReleased";
        default:                                return "Unknown";
    }
}

}