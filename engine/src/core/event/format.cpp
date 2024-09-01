#include "core/event/format.hpp"

namespace scsr
{

std::string FormatEvent(const Event& event)
{
    std::string format = "Unformatted event";

    switch (event.category)
    {
        case EventCategory::ECApplication:
        {
            format = "[Application]";
            switch (event.application.signal)
            {
                case ApplicationSignal::Quit:
                    format += " (Quit)";
                    break;
            }
            break;
        }
        case EventCategory::ECWindow:
        {
            format = "[Window]";
            format += " windowID: " + std::to_string(event.window.windowID);
            switch (event.window.signal)
            {
                case WindowSignal::Shown:
                    format += " (Shown)";
                    break;
                case WindowSignal::Hidden:
                    format += " (Hidden)";
                    break;
                case WindowSignal::Moved:
                    format += " (Moved)";
                    format += " x: " + std::to_string(event.window.x);
                    format += " y: " + std::to_string(event.window.y);
                    break;
                case WindowSignal::Resized:
                    format += " (Resized)";
                    format += " x: " + std::to_string(event.window.x);
                    format += " y: " + std::to_string(event.window.y);
                    break;
                case WindowSignal::Minimized:
                    format += " (Minimized)";
                    break;
                case WindowSignal::Maximized:
                    format += " (Maximized)";
                    break;
                case WindowSignal::Enter:
                    format += " (Enter)";
                    break;
                case WindowSignal::Leave:
                    format += " (Leave)";
                    break;
                case WindowSignal::FocusGained:
                    format += " (FocusGained)";
                    break;
                case WindowSignal::FocusLost:
                    format += " (FocusLost)";
                    break;
                case WindowSignal::Close:
                    format += " (Close)";
                    break;
            }
            break;
        }
        case EventCategory::ECKeyboard:
        {
            format = "[Keyboard]";
            switch (event.keyboard.state)
            {
                case ButtonState::Pressed:
                    format += " (Pressed)";
                    break;
                case ButtonState::Released:
                    format += " (Released)";
                    break;
            }
            format += " key: " + std::to_string(event.keyboard.keyCode);
            format += " repeat: " + std::to_string(event.keyboard.repeat);
            break;
        }
        case EventCategory::ECMouseMotion:
        {
            format = "[Mouse motion]";
            format += " x: " + std::to_string(event.mouseMotion.x);
            format += " y: " + std::to_string(event.mouseMotion.y);
            format += " dx: " + std::to_string(event.mouseMotion.dx);
            format += " dy: " + std::to_string(event.mouseMotion.dy);
            break;
        }
        case EventCategory::ECMouseButton:
        {
            format = "[Mouse button]";
            switch (event.mouseButton.state)
            {
                case ButtonState::Pressed:
                    format += " (Pressed)";
                    break;
                case ButtonState::Released:
                    format += " (Released)";
                    break;
            }
            format += " button: " + std::to_string(static_cast<u32>(event.mouseButton.button));
            format += " clicks: " + std::to_string(event.mouseButton.clicks);
            break;
        }
        case EventCategory::ECMouseWheel:
        {
            format = "[Mouse wheel]";
            format += " x: " + std::to_string(event.mouseWheel.x);
            format += " y: " + std::to_string(event.mouseWheel.y);
            break;
        }
        default:
        {
            break;
        }
    }

    return format;
}

}