set(CMAKE_CXX_STANDARD 20)

add_executable(runtime
    "src/main.cpp"
    "src/lua/binding.hpp"
    "src/lua/binding.cpp"
    "src/lua/script.hpp"
    "src/lua/script.cpp"
)

target_link_libraries(runtime PRIVATE engine)

if (SCSR_SCRIPTING)
    target_compile_definitions(runtime PUBLIC SCSR_SCRIPTING)
    target_link_libraries(runtime PRIVATE LuaLib)
endif()