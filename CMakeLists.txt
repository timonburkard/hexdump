cmake_minimum_required(VERSION 3.16)

project(hexdump
    VERSION 0.1.0
    LANGUAGES C
)

add_executable(hexdump
    src/hexdump.c
    ext/clic/src/clic.c
)

target_include_directories(hexdump PRIVATE
    ext/clic/src
)

target_compile_definitions(hexdump PRIVATE
    CLIC_NAME="hexdump"
    CLIC_DESCRIPTION="Create a hexadecimal dump of a file"
    CLIC_VERSION="v${PROJECT_VERSION}"
)

target_compile_options(hexdump PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wconversion
    -Wpedantic
    -Werror
)

set_target_properties(hexdump PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
