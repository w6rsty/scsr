macro(AddCoreTest name)
    add_executable(${name} ${name}.cpp)    
    target_link_libraries(${name} PRIVATE scsr.core)
    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}>)
    set_tests_properties(${name} PROPERTIES LABELS "console")
endmacro()

macro(AddGraphicsTest name)
    add_executable(${name} ${name}.cpp)    
    target_link_libraries(${name} PRIVATE scsr.graphics)
    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}>)
    set_tests_properties(${name} PROPERTIES LABELS "console")
endmacro()

set(CMAKE_CXX_STANDARD 20)

AddCoreTest(math)