set(tests include inlinevector)

link_libraries(microlib::microlib)

foreach(t IN LISTS tests)
    add_executable("test-${t}" "test-${t}")
    add_test("test-${t}" "test-${t}")
endforeach()