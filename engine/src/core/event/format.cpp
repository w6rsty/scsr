#include "core/event/format.hpp"

namespace scsr
{

std::string EventFormat(const Event& event)
{
    std::string format = "Unformatted event";

    switch (event.category)
    {
        case EventCategory::Application:
        {
            format = "Application event";
            switch (event.application.signal)
            {
                case ApplicationSignal::Quit:
                    format += " (Quit)";
                    break;
                case ApplicationSignal::Hidden:
                    format += " (Hidden)";
                    break;
                case ApplicationSignal::Enter:
                    format += " (Enter)";
                    break;
                case ApplicationSignal::Leave:
                    format += " (Leave)";
                    break;
                case ApplicationSignal::FocusGained:
                    format += " (Focus gained)";
                    break;
                case ApplicationSignal::FocusLost:
                    format += " (Focus lost)";
                    break;
            }
            break;
        }
        case EventCategory::Window:
        {
            format = "Window event";
            switch (event.window.signal)
            {
                case WindowSignal::Close:
                    format += " (Close)";
                    break;
                case WindowSignal::Move:
                    format += " (Move)";
                    break;
                case WindowSignal::Resize:
                    format += " (Resize)";
                    break;
            }
            format += " x: " + std::to_string(event.window.x);
            format += " y: " + std::to_string(event.window.y);
            break;
        }
        case EventCategory::Keyboard:
        {
            format = "Keyboard event";
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
        case EventCategory::MouseMotion:
        {
            format = "Mouse motion event";
            format += " x: " + std::to_string(event.mouseMotion.x);
            format += " y: " + std::to_string(event.mouseMotion.y);
            format += " dx: " + std::to_string(event.mouseMotion.dx);
            format += " dy: " + std::to_string(event.mouseMotion.dy);
            break;
        }
        case EventCategory::MouseButton:
        {
            format = "Mouse button event";
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
        case EventCategory::MouseWheel:
        {
            format = "Mouse wheel event";
            format += " offset: " + std::to_string(event.mouseWheel.offset);
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