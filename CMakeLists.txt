cmake_minimum_required(VERSION 3.20)
project(scsr LANGUAGES CXX C)

add_subdirectory(thirdparty)
add_subdirectory(engine)
add_subdirectory(runtime)
add_subdirectory(test)

option(SCSR_LOGGING "Enable logging" ON)
option(SCSR_SCRIPTING "Enable scripting" ON)

if (SCSR_LOGGING)
    message(STATUS "Logging enabled")
endif()
if (SCSR_SCRIPTING)
    message(STATUS "Scripting enabled")
endif()