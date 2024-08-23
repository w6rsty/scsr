---@class Window
local Window = {}

---@class binding
local binding = {}

---
--- Creates a new window.
--- @return Window window A handle to the newly created window.
function binding.CreateWindow()
    -- Implementation is provided by C++.
end

---
--- Destroys a window.
--- This is typically called by Lua's garbage collector.
--- @param window Window The window to be destroyed.
function binding.DestroyWindow(window)
    -- Implementation is provided by C++.
end

---
--- Updates the window.
--- This function is responsible for handling window events and rendering.
--- @param window Window The window to be updated.
function binding.WindowUpdate(window)
    -- Implementation is provided by C++.
end

return binding
