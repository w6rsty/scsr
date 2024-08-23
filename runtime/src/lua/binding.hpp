#pragma once

struct lua_State;

namespace binding
{

/// window = CreateWindow()
int CreateWindow(lua_State* L);
/// *** gc ***
int DestroyWindow(lua_State* L);
/// WindowUpdate(window)
int WindowUpdate(lua_State* L);

}
