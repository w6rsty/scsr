#include "binding.hpp"
#include "lua.hpp"

#include "graphics/window.hpp"

#include <new> // IWYU pragma: keep

namespace binding
{

int CreateWindow(lua_State* L)
{
    lua_newtable(L);

    void* ptr = lua_newuserdata(L, sizeof(scsr::Window));
    new (ptr) scsr::Window();

    luaL_getmetatable(L, "WindowMetaTable");
    lua_setmetatable(L, -2);
    return 1;
}

int DestroyWindow(lua_State* L)
{
    auto* window = static_cast<scsr::Window*>(lua_touserdata(L, 1));
    window->~Window();
    return 0;
}

int WindowUpdate(lua_State* L)
{
    auto* window = static_cast<scsr::Window*>(lua_touserdata(L, 1));
    window->OnUpdate();
    return 0;
}

}
