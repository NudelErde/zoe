#add Zoe as shared library

set(zoe_source src/zoe/core/Application.cpp src/zoe/core/Console.cpp src/zoe/core/File.cpp src/zoe/core/KeyCode.cpp src/zoe/core/Layer.cpp src/zoe/core/LayerStack.cpp src/zoe/parallel/Thread.cpp src/zoe/math/mat.cpp src/zoe/math/vec.cpp src/zoe/core/XMLParser.cpp src/platform/linux/LinuxInput.cpp src/platform/linux/LinuxWindow.cpp src/platform/OpenGL/OpenGLContext.cpp src/platform/OpenGL/OpenGLIndexBufferImpl.cpp src/platform/OpenGL/OpenGLRenderImpl.cpp src/platform/OpenGL/OpenGLShaderImpl.cpp src/platform/OpenGL/OpenGLTextureImpl.cpp src/platform/OpenGL/OpenGLVertexArrayImpl.cpp src/platform/OpenGL/OpenGLVertexBufferImpl.cpp src/platform/OpenGL/OpenGLVertexBufferLayoutImpl.cpp src/zoe/render/Material.cpp src/platform/windows/WindowsInput.cpp src/platform/windows/WindowsWindow.cpp src/zoe/render/Font.cpp src/platform/OpenGL/OpenGLRenderTargetImpl.cpp src/platform/OpenGL/OpenGLRenderTargetImpl.h src/zoe/render/GraphicsContext.cpp src/zoe/display/Component.cpp src/zoe/display/Game/Sprite.h src/zoe/display/Game/Sprite.cpp src/zoe/display/Camera.cpp src/zoe/display/Camera.h src/zoe/display/Game/WorldObject.cpp src/zoe/display/Game/WorldObject.h src/zoe/core/VirtualFiles.h src/zoe/core/VirtualFiles.cpp src/zoe/display/ComponentLayer.cpp src/zoe/display/ComponentLayer.h src/zoe/display/UI/UITool.cpp src/zoe/display/UI/UITool.h src/zoe/display/UI/Button.cpp src/zoe/display/UI/Button.h src/zoe/core/String.h src/zoe/render/Material.cpp src/zoe/render/Material.h src/zoe/render/Model.cpp src/zoe/render/Model.h src/zoe/display/Game/ModelComponent.cpp src/zoe/display/Game/ModelComponent.h src/zoe/display/ChaiScriptComponent.cpp src/zoe/display/ChaiScriptComponent.h src/zoe/display/NativeScriptComponent.cpp src/zoe/display/NativeScriptComponent.h src/zoe/display/UI/TextBox.cpp src/zoe/display/UI/TextBox.h src/zoe/core/UTF.cpp src/zoe/core/UTF.h src/zoe/core/Task.cpp src/zoe/core/Task.h src/zoe/core/Scheduler.cpp src/zoe/core/Scheduler.h src/zoe/display/Physics/PhysicsGroup.cpp src/zoe/display/Physics/PhysicsGroup.h src/zoe/display/Physics/PhysicsComponent.cpp src/zoe/display/Physics/PhysicsComponent.h src/zoe/display/Physics/AxisAlignedBox.cpp src/zoe/display/Physics/BoxCollider.cpp src/zoe/display/Physics/BoxCollider.h src/zoe/display/ScriptComponent.h src/zoe/display/UI/Image.cpp src/zoe/display/UI/Image.h)

add_library(zoe ${zoe_source})

#add library to zoe
list(APPEND ZOE_INCLUDES ${PROJECT_SOURCE_DIR}/dependencies/ChaiScript/include)
target_include_directories(zoe PRIVATE "${ZOE_INCLUDES}")
IF(CMAKE_BUILD_TYPE STREQUAL Debug)
  target_compile_definitions(zoe PRIVATE ZOE_DEBUG)
ENDIF()

add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/zoe/src/zoe/core/VirtualFiles.cpp
        WORKING_DIRECTORY  ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -P "${CMAKE_SOURCE_DIR}/zoe/src/internal/setup/preBuild.cmake")
add_custom_command(TARGET zoe POST_BUILD
        WORKING_DIRECTORY  ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -P "${CMAKE_SOURCE_DIR}/zoe/src/internal/setup/postBuild.cmake")

IF(WIN32)
  add_dependencies(zoe GLEW glew_download)
  add_dependencies(zoe webp)
  add_dependencies(zoe freetype)
  target_compile_options(zoe PRIVATE /std:c++latest)
  target_link_libraries(zoe glfw GLEW opengl32 webp freetype)
ELSE()
  target_compile_options(zoe PUBLIC -fcoroutines)
  target_link_libraries(zoe pthread glfw GL GLEW webp freetype)
ENDIF()