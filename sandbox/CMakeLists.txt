add_definitions(-DZOE_SHARED)

add_executable(sandbox Main.cpp)
target_include_directories(sandbox PRIVATE ${PROJECT_SOURCE_DIR}/zoe/src)

if(UNIX)
    #set_target_properties(sandbox PROPERTIES LINK_FLAGS "-Wl,rpath=.")
endif()

add_dependencies(sandbox zoe)
target_link_libraries(sandbox zoe)