#ifdef SCSR_SCRIPTING

#include "script.hpp"
#include "core/log.hpp"

#include <cstdlib>
#include <cassert>
#include <string>

void* ScriptAllocate(void* ud, void* ptr, size_t osize, size_t nsize)
{
    auto* script = static_cast<LuaScript*>(ud);
    if (nsize == 0)
    {
        script->AllocatedMemory -= osize;
        free(ptr);
        return nullptr;
    }
    else
    {
        if (script->AllocatedMemory + nsize - osize > script->MemoryLimit)
        {
            // LOG_ERROR(
            //     "Memory limit exceeded: {} > {}",
            //     script->AllocatedMemory + nsize - osize, script->MemoryLimit
            // );
            return nullptr;
        }
        script->AllocatedMemory += nsize - osize;
        return realloc(ptr, nsize);
    }
}

LuaScript::LuaScript(bool useLib, usize memoryLimit) :
    MemoryLimit(memoryLimit),
    AllocatedMemory(0)
{
    L = lua_newstate(ScriptAllocate, this);
    if (useLib)
    {
        luaL_openlibs(L);
    }
}

LuaScript::~LuaScript()
{
    lua_close(L);
}

LuaScript& LuaScript::AddLines(std::function<void(lua_State*)> fn)
{
    fn(L);
    return *this;
}

LuaScript& LuaScript::AddMetaMethod(const std::string& metatable, const std::string& name, LuaCFn fn)
{
    luaL_newmetatable(L, metatable.data());
    lua_pushstring(L, name.data());
    lua_pushcfunction(L, fn);
    lua_settable(L, -3);

    return *this;
}


void LuaScript::Load(const std::string& script)
{
    int result = luaL_dostring(L, script.data());
    if (result != LUA_OK)
    {
        // LOG_ERROR("Failed to load script: {}", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

void LuaScript::RegisterFn(const std::string& name, LuaCFn fn)
{
    lua_register(L, name.data(), fn);
}

#endif