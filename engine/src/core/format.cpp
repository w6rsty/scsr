#include "core/format.hpp"

namespace scsr
{

std::string FormatKeyBoardKeyCode(i32 code)
{
    std::string format;
#define KEY_CODE_CASE(code) case KeyboardKeyCode::code: format = #code; break;
    switch (code)
    {
    KEY_CODE_CASE(KeyTab)
    KEY_CODE_CASE(KeyEnter)
    KEY_CODE_CASE(KeyCapsLock)
    KEY_CODE_CASE(KeyEscape)
    KEY_CODE_CASE(KeySpace)
    KEY_CODE_CASE(Key0)
    KEY_CODE_CASE(Key1)
    KEY_CODE_CASE(Key2)
    KEY_CODE_CASE(Key3)
    KEY_CODE_CASE(Key4)
    KEY_CODE_CASE(Key5)
    KEY_CODE_CASE(Key6)
    KEY_CODE_CASE(Key7)
    KEY_CODE_CASE(Key8)
    KEY_CODE_CASE(Key9)
    KEY_CODE_CASE(KeyA)
    KEY_CODE_CASE(KeyB)
    KEY_CODE_CASE(KeyC)
    KEY_CODE_CASE(KeyD)
    KEY_CODE_CASE(KeyE)
    KEY_CODE_CASE(KeyF)
    KEY_CODE_CASE(KeyG)
    KEY_CODE_CASE(KeyH)
    KEY_CODE_CASE(KeyI)
    KEY_CODE_CASE(KeyJ)
    KEY_CODE_CASE(KeyK)
    KEY_CODE_CASE(KeyL)
    KEY_CODE_CASE(KeyM)
    KEY_CODE_CASE(KeyN)
    KEY_CODE_CASE(KeyO)
    KEY_CODE_CASE(KeyP)
    KEY_CODE_CASE(KeyQ)
    KEY_CODE_CASE(KeyR)
    KEY_CODE_CASE(KeyS)
    KEY_CODE_CASE(KeyT)
    KEY_CODE_CASE(KeyU)
    KEY_CODE_CASE(KeyV)
    KEY_CODE_CASE(KeyW)
    KEY_CODE_CASE(KeyX)
    KEY_CODE_CASE(KeyY)
    KEY_CODE_CASE(KeyZ)
    KEY_CODE_CASE(KeyLeft)
    KEY_CODE_CASE(KeyRight)
    KEY_CODE_CASE(KeyUp)
    KEY_CODE_CASE(KeyDown)
    KEY_CODE_CASE(KeyLeftControl)
    KEY_CODE_CASE(KeyLeftShift)
    KEY_CODE_CASE(KeyLeftAlt)
    KEY_CODE_CASE(KeySuper)
    KEY_CODE_CASE(KeyRightControl)
    KEY_CODE_CASE(KeyRightShift)
    KEY_CODE_CASE(KeyRightAlt)
    default:
        format = std::to_string(code);
    }
    return format;
}

std::string FormatMouseButtonCode(u8 code)
{
    std::string format;
    switch (code)
    {
    case MouseButtonCode::Left:
        format = "Left";
        break;
    case MouseButtonCode::Middle:
        format = "Middle";
        break;
    case MouseButtonCode::Right:
        format = "Right";
        break;
    case MouseButtonCode::X1:
        format = "X1";
        break;
    case MouseButtonCode::X2:
        format = "X2";
        break;
      break;
    }
    return format;
}

std::string FormatEvent(const Event& event)
{
    std::string format = "[Event] ";

    switch (event.type)
    {
    case EventType::None:
        break;
    case EventType::AppExit:
        format += "AppExit";
        break;
    case EventType::WindowShown:
        format += "WindowShown windowID: " + std::to_string(event.windowShow.windowID);
        break;
    case EventType::WindowMoved:
        format += "WindowMoved windowID: " + std::to_string(event.windowMove.windowID)
            + " x: " + std::to_string(event.windowMove.x)
            + " y: " + std::to_string(event.windowMove.y);
        break;
    case EventType::WindowResized:
        format += "WindowResized windowID: " + std::to_string(event.windowResize.windowID)
            + " x: " + std::to_string(event.windowResize.x)
            + " y: " + std::to_string(event.windowResize.y);
        break;
    case EventType::WindowMinimized:
        format += "WindowMinimized windowID: " + std::to_string(event.windowMinimize.windowID);
        break;
    case EventType::WindowMaximized:
        format += "WindowMaximized windowID: " + std::to_string(event.windowMaximize.windowID);
        break;
    case EventType::WindowEnter:
        format += "WindowEnter windowID: " + std::to_string(event.windowEnter.windowID);
        break;
    case EventType::WindowLeave:
        format += "WindowLeave windowID: " + std::to_string(event.windowLeave.windowID);
        break;
    case EventType::WindowFocusGained:
        format += "WindowFocusGained windowID: " + std::to_string(event.windowFocusGained.windowID);
        break;
    case EventType::WindowFocusLost:
        format += "WindowFocusLost windowID: " + std::to_string(event.windowFocusLost.windowID);
        break;
    case EventType::WindowClose:
        format += "WindowClose windowID: " + std::to_string(event.windowClose.windowID);
        break;
    case EventType::KeyboardPressed:
        format += "KeyboardPressed keyCode: " + FormatKeyBoardKeyCode((KeyboardKeyCode)event.keyboardPressed.keyCode)
            + " repeat: " + std::to_string(event.keyboardPressed.repeat);
        break;
    case EventType::KeyboardReleased:
        format += "KeyboardReleased keyCode: " + FormatKeyBoardKeyCode((KeyboardKeyCode)event.keyboardReleased.keyCode)
            + " repeat: " + std::to_string(event.keyboardReleased.repeat);
        break;
    case EventType::MouseMotion:
        format += "MouseMotion x: " + std::to_string(event.mouseMotion.x)
            + " y: " + std::to_string(event.mouseMotion.y);
        break;
    case EventType::MouseButtonPressed:
        format += "MouseButtonPressed button: " + FormatMouseButtonCode((MouseButtonCode)event.mouseButtonPressed.button)
            + " (" + std::to_string(event.mouseButtonPressed.cx) + ", " + std::to_string(event.mouseButtonPressed.cy) + ")"
            + " clicks: " + std::to_string(event.mouseButtonPressed.clicks);
        break;
    case EventType::MouseButtonReleased:
        format += "MouseButtonReleased button: " + FormatMouseButtonCode((MouseButtonCode)event.mouseButtonPressed.button)
            + " (" + std::to_string(event.mouseButtonReleased.cx) + ", " + std::to_string(event.mouseButtonReleased.cy) + ")"
            + " clicks: " + std::to_string(event.mouseButtonReleased.clicks);
        break;
    case EventType::MouseWheel:
        format += "MouseWheel x: " + std::to_string(event.mouseWheel.x)
            + " y: " + std::to_string(event.mouseWheel.y)
            + " (" + std::to_string(event.mouseWheel.cx) + ", " + std::to_string(event.mouseWheel.cy) + ")";
      break;
    }

    return format;
}

}