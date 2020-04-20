#add Zoe as shared library
add_definitions(-DGLEW_STATIC)
IF(WIN32)
  add_definitions(/bigobj)
ENDIF()

add_definitions(-DZOE_SHARED -DBUILDING_DLL)
add_library(zoe SHARED src/zoe/Sort.cpp src/zoe/Application.cpp src/zoe/Console.cpp src/zoe/File.cpp src/zoe/KeyCode.cpp src/zoe/Layer.cpp src/zoe/LayerStack.cpp src/zoe/UI/UILayer.cpp src/zoe/UI/Ellipse.cpp src/zoe/UI/CommonUI.cpp src/zoe/UI/Image.cpp src/zoe/parallel/Thread.cpp src/zoe/parallel/Parallel.cpp src/zoe/math/mat.cpp src/zoe/math/vec.cpp src/zoe/game/nodes/2DGameNode.cpp src/zoe/game/nodes/Node.cpp src/zoe/game/nodes/ScriptNode.cpp src/zoe/game/nodes/SpriteNode.cpp src/zoe/game/nodes/TextureNode.cpp src/zoe/core/XMLParser.cpp src/platform/linux/LinuxInput.cpp src/platform/linux/LinuxWindow.cpp src/platform/OpenGL/OpenGLContext.cpp src/platform/OpenGL/OpenGLIndexBufferImpl.cpp src/platform/OpenGL/OpenGLRenderImpl.cpp src/platform/OpenGL/OpenGLShaderImpl.cpp src/platform/OpenGL/OpenGLTextureImpl.cpp src/platform/OpenGL/OpenGLVertexArrayImpl.cpp src/platform/OpenGL/OpenGLVertexBufferImpl.cpp src/platform/OpenGL/OpenGLVertexBufferLayoutImpl.cpp src/zoe/game/3D/Layer3D.cpp src/zoe/game/3D/Layer3D.h src/zoe/game/3D/Object3D.cpp src/zoe/game/3D/Object3D.h src/zoe/render/Camera.cpp src/zoe/render/Camera.h src/zoe/render/Material.cpp src/zoe/game/3D/Line3D.cpp src/zoe/game/3D/Line3D.h src/zoe/render/Model.cpp src/zoe/render/Model.h)
#GLEW
IF(WIN32)
  message("Prepare GLEW download")
  include(ExternalProject)
  ExternalProject_Add(glew_download
    URL               https://netix.dl.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0-win32.zip
    BUILD_COMMAND ""
    CONFIGURE_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    )
  file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/zoe/glew_download-prefix/src/glew_download/include)
  file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/zoe/glew_download-prefix/src/glew_download/lib/Release/x64/)
  add_library(GLEW STATIC IMPORTED DEPENDS glew_download)
  set_target_properties(GLEW PROPERTIES
        IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/zoe/glew_download-prefix/src/glew_download/lib/Release/x64/glew32s.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/zoe/glew_download-prefix/src/glew_download/include")
  add_dependencies(zoe GLEW glew_download)
ELSE()
  #should already have GLEW
ENDIF()

#create glfw files
message("Create glfw files")
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/build/glfw)
execute_process(COMMAND ${CMAKE_COMMAND} -DBUILD_SHARED_LIBS=off ${PROJECT_SOURCE_DIR}/dependencies/glfw WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build/glfw)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_SOURCE_DIR}/build/glfw)
message("Done creating glfw files")

#make glfw library
add_library(glfw STATIC IMPORTED)
IF (WIN32)
  set_target_properties(glfw PROPERTIES
          IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/build/glfw/src/Debug/glfw3.lib"
          INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/dependencies/glfw/include")
ELSE()
  set_target_properties(glfw PROPERTIES
          IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/build/glfw/src/libglfw3.a"
          INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/dependencies/glfw/include")
ENDIF()


#include ChaiScriptSource
target_include_directories(zoe PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/ChaiScript/include)

#create webp
message("Create webp files")
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/build/webp)
execute_process(COMMAND ${CMAKE_COMMAND} -Wno-dev ${PROJECT_SOURCE_DIR}/dependencies/webp WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build/webp)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_SOURCE_DIR}/build/webp)
#make webp library
add_library(webp STATIC IMPORTED)
IF (WIN32)
  set_target_properties(webp PROPERTIES
          IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/build/webp/Debug/webp.lib"
          INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/dependencies/webp/src")
ELSE()
  set_target_properties(webp PROPERTIES
          IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/build/webp/libwebp.a"
          INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/dependencies/webp/src")
ENDIF()
message("Done creating webp files")

#add library to zoe
IF(WIN32)
  target_link_libraries(zoe glfw GLEW opengl32 webp)
ELSE()
  target_link_libraries(zoe pthread glfw GL GLEW webp)
ENDIF()

if(WIN32)
  add_custom_command(TARGET zoe POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:zoe> ${PROJECT_BINARY_DIR}/sandbox/zoe.dll
          )
endif()