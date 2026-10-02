if(NOT DEFINED HEXDUMP)
    message(FATAL_ERROR "HEXDUMP is required")
endif()

if(NOT DEFINED WORK_DIR)
    message(FATAL_ERROR "WORK_DIR is required")
endif()

if(NOT DEFINED TEST_MODE)
    message(FATAL_ERROR "TEST_MODE is required")
endif()

file(MAKE_DIRECTORY "${WORK_DIR}")

function(check_output_file expected_file actual_output)
    file(READ "${expected_file}" expected_output)
    string(REPLACE "\r\n" "\n" actual_output "${actual_output}")
    string(REPLACE "\r\n" "\n" expected_output "${expected_output}")
    string(REGEX REPLACE "\n$" "" actual_output "${actual_output}")
    string(REGEX REPLACE "\n$" "" expected_output "${expected_output}")
    if(NOT actual_output STREQUAL expected_output)
        message(FATAL_ERROR "Unexpected output:\n--- expected ---\n${expected_output}\n--- actual ---\n${actual_output}\n--- end ---")
    endif()
endfunction()

if(TEST_MODE STREQUAL "file")
    execute_process(
        COMMAND "${HEXDUMP}" show "${CMAKE_CURRENT_LIST_DIR}/input/file.bin" --length 16 --width 8
        RESULT_VARIABLE exit_code
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error_output
    )

    if(NOT exit_code EQUAL 0)
        message(FATAL_ERROR "hexdump failed: ${error_output}")
    endif()
    check_output_file("${CMAKE_CURRENT_LIST_DIR}/expected/file.txt" "${output}")
endif()

if(TEST_MODE STREQUAL "offset_length")
    execute_process(
        COMMAND "${HEXDUMP}" show "${CMAKE_CURRENT_LIST_DIR}/input/offset.bin" --offset 0x10 --length 16 --width 8
        RESULT_VARIABLE exit_code
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error_output
    )

    if(NOT exit_code EQUAL 0)
        message(FATAL_ERROR "hexdump failed: ${error_output}")
    endif()
    check_output_file("${CMAKE_CURRENT_LIST_DIR}/expected/offset_length.txt" "${output}")
endif()

if(TEST_MODE STREQUAL "stdin")
    execute_process(
        COMMAND "${HEXDUMP}" show - --length 16 --width 8
        INPUT_FILE "${CMAKE_CURRENT_LIST_DIR}/input/stdin.txt"
        RESULT_VARIABLE exit_code
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error_output
    )

    if(NOT exit_code EQUAL 0)
        message(FATAL_ERROR "hexdump failed: ${error_output}")
    endif()
    check_output_file("${CMAKE_CURRENT_LIST_DIR}/expected/stdin.txt" "${output}")
endif()

if(TEST_MODE STREQUAL "binary")
    execute_process(
        COMMAND "${HEXDUMP}" show "${CMAKE_CURRENT_LIST_DIR}/input/binary.bin" --length 13 --width 8
        RESULT_VARIABLE exit_code
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error_output
    )

    if(NOT exit_code EQUAL 0)
        message(FATAL_ERROR "hexdump failed: ${error_output}")
    endif()
    check_output_file("${CMAKE_CURRENT_LIST_DIR}/expected/binary.txt" "${output}")
endif()

if(NOT TEST_MODE MATCHES "^(file|offset_length|stdin|binary)$")
    message(FATAL_ERROR "Unsupported TEST_MODE: ${TEST_MODE}")
endif()
