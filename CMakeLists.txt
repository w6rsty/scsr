cmake_minimum_required(VERSION 3.20)
project(scsr LANGUAGES CXX C)

add_subdirectory(thirdparty)

message("========================================")

option(SCSR_LOGGING "Enable logging" ON)
option(SCSR_SCRIPTING "Enable scripting" OFF)
option(SCSR_RT_ASSERT "Enable runtime assert" ON)
option(SCSR_TRACY "Enable tracy profiling" ON)

if (SCSR_LOGGING)
    message(STATUS "Logging enabled")
endif()
if (SCSR_SCRIPTING)
    message(STATUS "Scripting enabled")
endif()
if (SCSR_RT_ASSERT)
    message(STATUS "Runtime assert enabled")
endif()
if (SCSR_TRACY)
    message(STATUS "Enable tracy profiling")
    # add_compile_definitions(TRACY_ENABLE)
endif()

add_subdirectory(engine)
add_subdirectory(runtime)
add_subdirectory(test)