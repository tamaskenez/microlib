file(GLOB_RECURSE headers *.h)
add_library(microlib
    ${headers}
    ul.cpp)

target_include_directories(microlib
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

add_library(microlib::microlib ALIAS microlib)
