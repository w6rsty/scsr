--[[
const char* startFile = "runtime/src/script/start.lua";

auto start = ReadFile(startFile);

if (start.has_value())
{
    LuaScript script;
    script.RegisterFn("CreateWindow", binding::CreateWindow);
    script.AddMetaMethod("WindowMetaTable", "__gc", binding::DestroyWindow);
    script.RegisterFn("WindowUpdate", binding::WindowUpdate);
    script.Load(start.value());

]]

local window = CreateWindow()

for  i = 1, 100 do
    WindowUpdate(window)
end