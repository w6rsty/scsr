set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE ENGINE_SRC "src/*.cpp")
file(GLOB_RECURSE ENGINE_HDR "include/*.hpp")

add_library(engine STATIC ${ENGINE_SRC})
target_include_directories(engine PUBLIC include)

target_link_libraries(engine SDL2::SDL2-static SDL2::SDL2main STB)

if (SCSR_LOGGING)
    target_compile_definitions(engine PUBLIC SCSR_LOGGING)
    target_link_libraries(engine fmt)
endif()

