add_executable(sandbox Main.cpp)
target_include_directories(sandbox PRIVATE ${PROJECT_SOURCE_DIR}/zoe/src)

add_dependencies(sandbox zoe)
target_link_libraries(sandbox zoe)