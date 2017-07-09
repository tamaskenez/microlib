set(tests
	include
	inlinevector
	alg_bitset
	alg_cont_eq_fun
	alg_elementwise
	alg_scalar_eq_fun
	to_string
	type_traits)

set(tests2
	ul
	algorithm
    math
    string
    string_par
)

link_libraries(microlib::microlib)

foreach(t IN LISTS tests)
	set(b "test-${t}")
	set(n "${b}")
    add_executable(${n} ${b})
    add_test(${n} ${n})
	set(n "${b}-ul")
    add_executable(${n} ${b})
    target_compile_definitions(${n} PRIVATE ULTEST_USING_UL)
    add_test(${n} ${n})
	set(n "${b}-std")
    add_executable(${n} ${b})
    target_compile_definitions(${n} PRIVATE ULTEST_USING_STD)
    add_test(${n} ${n})
	set(n "${b}-std-ul")
    add_executable(${n} ${b})
    target_compile_definitions(${n} PRIVATE ULTEST_USING_UL ULTEST_USING_STD)
    add_test(${n} ${n})
endforeach()

foreach(t IN LISTS tests2)
	set(b "test-${t}")
	set(n "${b}")
    message(STATUS "add_executable(${n} \"${b}.cpp\")")
    add_executable(${n} "${b}.cpp")
    message(STATUS "add_test(${n} ${n})")
    add_test(${n} ${n})
endforeach()
