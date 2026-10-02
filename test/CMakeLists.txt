add_test(
    NAME hexdump_file
    COMMAND ${CMAKE_COMMAND}
        -DHEXDUMP=$<TARGET_FILE:hexdump>
        -DTEST_MODE=file
        -DWORK_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/check_hexdump.cmake
)

add_test(
    NAME hexdump_offset_length
    COMMAND ${CMAKE_COMMAND}
        -DHEXDUMP=$<TARGET_FILE:hexdump>
        -DTEST_MODE=offset_length
        -DWORK_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/check_hexdump.cmake
)

add_test(
    NAME hexdump_stdin
    COMMAND ${CMAKE_COMMAND}
        -DHEXDUMP=$<TARGET_FILE:hexdump>
        -DTEST_MODE=stdin
        -DWORK_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/check_hexdump.cmake
)

    add_test(
        NAME hexdump_binary
        COMMAND ${CMAKE_COMMAND}
        -DHEXDUMP=$<TARGET_FILE:hexdump>
        -DTEST_MODE=binary
        -DWORK_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/check_hexdump.cmake
    )
