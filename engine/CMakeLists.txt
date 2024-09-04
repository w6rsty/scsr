set(scsr_include_dir include)

# core
aux_source_directory(src/core scsr_core_common_src)
file(GLOB scsr_core_common_hdr include/core/*.hpp)

aux_source_directory(src/core/ds scsr_core_ds_src)
file(GLOB scsr_core_ds_hdr include/core/ds/*.hpp)

aux_source_directory(src/core/event scsr_core_event_src)
file(GLOB scsr_core_event_hdr include/core/event/*.hpp)

aux_source_directory(src/core/math scsr_core_math_src)
file(GLOB scsr_core_math_hdr include/core/math/*.hpp)

set(scsr_core_src
    ${scsr_core_common_src}
    ${scsr_core_ds_src}
    ${scsr_core_event_src}
    ${scsr_core_math_src}
)

set(scsr_core_hdr
    ${scsr_core_common_hdr}
    ${scsr_core_ds_hdr}
    ${scsr_core_event_hdr}
    ${scsr_core_math_hdr}
)

add_library(scsr.core STATIC)
target_sources(scsr.core PRIVATE ${scsr_core_src})
target_sources(scsr.core PUBLIC ${scsr_core_hdr})
target_include_directories(scsr.core PUBLIC ${scsr_include_dir})
target_compile_features(scsr.core PUBLIC cxx_std_20)
target_link_libraries(scsr.core PUBLIC fmt SDL2::SDL2-static)
target_compile_definitions(scsr.core PUBLIC "$<$<CONFIG:Debug>:SCSR_LOGGING>" SCSR_LOGGING)
target_compile_definitions(scsr.core PUBLIC "$<$<CONFIG:Debug>:SCSR_RT_ASSERT>" SCSR_RT_ASSERT)
# check AVX2 compiler support
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
# check AVX2 platform support
include(CheckCXXSourceCompiles)
check_cxx_source_compiles("#include <immintrin.h>\nint main() { __m256i a = _mm256_setzero_si256(); return 0; }" COMPILER_SUPPORTS_AVX2)
if(COMPILER_SUPPORTS_AVX2 AND COMPILER_SUPPORTS_AVX2)
    message(STATUS "AVX2 support enabled")
    target_compile_options(scsr.core PUBLIC "-mavx2")
    target_compile_definitions(scsr.core PUBLIC SCSR_AVX2)
else()
    message(WARNING "AVX2 is not supported.")
endif()

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