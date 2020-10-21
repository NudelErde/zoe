cmake_minimum_required(VERSION 3.13)

project(Zoe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/webp)

if (WIN32)
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/build/freetype)
    execute_process(
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/freetype
            COMMAND ${CMAKE_COMMAND} -B ${PROJECT_BINARY_DIR}/build/freetype -D CMAKE_BUILD_TYPE=DEBUG
    )
    execute_process(
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/freetype
            COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/build/freetype
    )
    add_library(freetype STATIC IMPORTED)
    set_target_properties(freetype PROPERTIES
            IMPORTED_LOCATION "${PROJECT_BINARY_DIR}/build/freetype/Debug/freetyped.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/dependencies/freetype/include")
else ()
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/build/freetype)
    execute_process(
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/freetype
            COMMAND ${CMAKE_COMMAND} -B ${PROJECT_BINARY_DIR}/build/freetype -D CMAKE_BUILD_TYPE=DEBUG -DCMAKE_DISABLE_FIND_PACKAGE_HarfBuzz=TRUE -DCMAKE_DISABLE_FIND_PACKAGE_BZip2=TRUE -DCMAKE_DISABLE_FIND_PACKAGE_PNG=TRUE -DCMAKE_DISABLE_FIND_PACKAGE_ZLIB=TRUE
    )
    execute_process(
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/freetype
            COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/build/freetype
    )
    add_library(freetype STATIC IMPORTED)
    set_target_properties(freetype PROPERTIES
            IMPORTED_LOCATION "${PROJECT_BINARY_DIR}/build/freetype/libfreetyped.a"
            INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/dependencies/freetype/include")
endif ()

add_definitions(-DGLEW_STATIC -DGLFW_STATIC)
IF (WIN32)
    add_definitions(/bigobj)
ENDIF ()

set(ZOE_INCLUDES "")
#add_definitions(-DZOE_SHARED -DBUILDING_DLL)
#GLEW
IF (WIN32)
    include(ExternalProject)
    ExternalProject_Add(glew_download
            URL https://netix.dl.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0-win32.zip
            BUILD_COMMAND ""
            CONFIGURE_COMMAND ""
            INSTALL_COMMAND ""
            TEST_COMMAND ""
            )
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/glew_download-prefix/src/glew_download/include)
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/glew_download-prefix/src/glew_download/lib/Release/x64/)
    add_library(GLEW STATIC IMPORTED DEPENDS glew_download)
    set_target_properties(GLEW PROPERTIES
            IMPORTED_LOCATION "${PROJECT_BINARY_DIR}/glew_download-prefix/src/glew_download/lib/Release/x64/glew32s.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BINARY_DIR}/glew_download-prefix/src/glew_download/include/")
ELSE ()
    #should already have GLEW
ENDIF ()

list(APPEND ZOE_INCLUDES ${PROJECT_SOURCE_DIR}/dependencies/webp/src)

#Engine
add_subdirectory(zoe)

#Sandbox
add_subdirectory(sandbox)

#tests
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    add_subdirectory(tests)
endif ()

#docs
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc_doxygen ALL
            COMMAND ${CMAKE_COMMAND} -E env DOXYGEN_DOC_DIR=${CMAKE_BINARY_DIR}/docs ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/.doxyfile
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
endif()