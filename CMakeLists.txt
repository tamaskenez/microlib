cmake_minimum_required(VERSION 3.1)
project(microlib)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

set(common_gcc_clang -Wno-shorten-64-to-32 -Wno-sign-compare
    -Wno-sign-conversion -Werror=return-type -Wformat=2 )
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Using Clang compile options")
    add_compile_options(-Weverything
        -Wno-c++98-compat -Wno-old-style-cast -Wno-deprecated
        -Wno-c++98-compat-pedantic -Wno-padded
        -Wno-global-constructors -Wno-documentation-unknown-command
        -Wno-implicit-fallthrough -Wno-weak-vtables
        -Wno-shadow -Wno-cast-align
        -Wno-conversion -Wno-double-promotion
        -Wno-covered-switch-default
        -Wextra -pedantic
        ${common_gcc_clang}
        -Wall -Wnon-virtual-dtor
        -Wunused -Woverloaded-virtual 
        )
elseif(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Using Gnu compile options")
    add_compile_options(-Wall -Wextra -Wnon-virtual-dtor -pedantic
        -Wunused -Woverloaded-virtual 
        ${common_gcc_clang})
elseif(MSVC)
    message(STATUS "Using MSVC compile options")
    add_compile_options(/W4 /W44640 /w14242 /w14254 /w14263 /w14265 /w14287
        /we4289 /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619
        /w14640 /w14826 /w14905 /w14906 /w14928)
endif()

enable_testing()

add_subdirectory(ul)
add_subdirectory(test)
add_subdirectory(playground)

