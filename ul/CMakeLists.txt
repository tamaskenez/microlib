file(GLOB_RECURSE headers *.h)
add_library(microlib
    ${headers}
    ul.cpp
    string.cpp
    check.cpp)

target_include_directories(microlib
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

add_library(microlib::microlib ALIAS microlib)

install(TARGETS microlib EXPORT microlib-targets
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(EXPORT microlib-targets
    DESTINATION lib/cmake/microlib
    NAMESPACE microlib::
    FILE microlib-config.cmake
)

install(DIRECTORY .
    DESTINATION include/ul
    FILES_MATCHING PATTERN *.h
    )

