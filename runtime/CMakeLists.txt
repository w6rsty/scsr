aux_source_directory(src scsr_runtime_src)

add_executable(runtime)
target_sources(runtime PRIVATE ${scsr_runtime_src})
target_compile_features(runtime PRIVATE cxx_std_20)
target_link_libraries(runtime PRIVATE scsr.scsr)
