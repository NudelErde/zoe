macro(add_test_file TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    add_test(NAME ${TESTNAME} COMMAND $<TARGET_FILE:${TESTNAME}>)
    target_include_directories(${TESTNAME} PRIVATE ${PROJECT_SOURCE_DIR}/zoe/src)
    target_link_libraries(${TESTNAME} zoe)
endmacro()

add_test_file(matrixTest matrixTest.cpp)
add_test_file(fileTest fileTest.cpp)
