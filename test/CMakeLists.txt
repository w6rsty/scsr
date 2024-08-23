macro(AddConsoleTest name)
    add_executable(${name} ${name}.cpp)    
    target_link_libraries(${name} PRIVATE engine)
    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}>)
    set_tests_properties(${name} PROPERTIES LABELS "console")
endmacro()

set(CMAKE_CXX_STANDARD 20)

AddConsoleTest(index)
AddConsoleTest(event)