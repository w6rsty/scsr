macro(AddCoreTest name)
    add_executable(${name} ${name}.cpp)    
    target_link_libraries(${name} PRIVATE scsr.core)
    target_compile_features(${name} PUBLIC cxx_std_20)
    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}>)
    set_tests_properties(${name} PROPERTIES LABELS "console")
endmacro()

macro(AddGraphicsTest name)
    add_executable(${name} ${name}.cpp)    
    target_link_libraries(${name} PRIVATE scsr.graphics)
    target_compile_features(${name} PUBLIC cxx_std_20)

    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}>)
    set_tests_properties(${name} PROPERTIES LABELS "console")
endmacro()

AddCoreTest(math)

AddGraphicsTest(shader)
AddGraphicsTest(gltf)