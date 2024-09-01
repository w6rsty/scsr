set(scsr_include_dir include)

# core
aux_source_directory(src/core scsr_core_src)
aux_source_directory(src/core/event scsr_core_src)
file(GLOB_RECURSE scsr_core_hdr include/core/*.hpp)

add_library(scsr.core STATIC)
target_sources(scsr.core PRIVATE ${scsr_core_src})
target_sources(scsr.core PUBLIC ${scsr_core_hdr})
target_include_directories(scsr.core PUBLIC ${scsr_include_dir})
target_compile_features(scsr.core PUBLIC cxx_std_20)
target_link_libraries(scsr.core PUBLIC fmt SDL2::SDL2-static)
target_compile_definitions(scsr.core PUBLIC "$<$<CONFIG:Debug>:SCSR_LOGGING>" SCSR_LOGGING)
target_compile_definitions(scsr.core PUBLIC "$<$<CONFIG:Debug>:SCSR_RT_ASSERT>" SCSR_RT_ASSERT)

# graphics
aux_source_directory(src/graphics scsr_graphics_src)
file(GLOB_RECURSE scsr_graphics_hdr include/graphics/*.hpp)

add_library(scsr.graphics STATIC)
target_sources(scsr.graphics PRIVATE ${scsr_graphics_src})
target_sources(scsr.graphics PUBLIC ${scsr_graphics_hdr})
target_include_directories(scsr.graphics PUBLIC ${scsr_include_dir})
target_compile_features(scsr.graphics PUBLIC cxx_std_20)
target_link_libraries(scsr.graphics PUBLIC scsr.core)

# entrypoint
add_library(scsr.engine STATIC)
target_sources(scsr.engine PRIVATE src/entrypoint.cpp)

# interface
add_library(scsr.scsr INTERFACE)
target_link_libraries(scsr.scsr INTERFACE
    scsr.core
    scsr.graphics
    scsr.engine)