#pragma once

#ifdef SCSR_SCRIPTING

#include "core/type.hpp"

#include "lua.hpp"

#include <string>
#include <functional>

using namespace scsr;

static const usize SCRIPT_MAX_MEMORY = 1024 * 1024 * 10;

void* ScriptAllocate(void* ud, void* ptr, usize osize, usize nsize);

using LuaCFn = int(*)(lua_State*);

struct LuaScript
{
    lua_State* L;
    usize MemoryLimit;
    usize AllocatedMemory;

   LuaScript(bool useLib = false, usize memoryLimit = SCRIPT_MAX_MEMORY);
    ~LuaScript();
    /// Call user define Lua CAPI
    LuaScript& AddLines(std::function<void(lua_State*)> fn);
    /// Make sure using correct metamehod
    LuaScript& AddMetaMethod(const std::string& metatable, const std::string& name, LuaCFn fn);
    void RegisterFn(const std::string& name, LuaCFn fn);

    /// Load script from string and execute
    void Load(const std::string& script);

};

#endif