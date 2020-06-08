#add Zoe as shared library

add_library(zoe src/zoe/Sort.cpp src/zoe/Application.cpp src/zoe/Console.cpp src/zoe/File.cpp src/zoe/KeyCode.cpp src/zoe/Layer.cpp src/zoe/LayerStack.cpp src/zoe/UI/UILayer.cpp src/zoe/UI/Ellipse.cpp src/zoe/UI/CommonUI.cpp src/zoe/UI/Image.cpp src/zoe/parallel/Thread.cpp src/zoe/parallel/Parallel.cpp src/zoe/math/mat.cpp src/zoe/math/vec.cpp src/zoe/game/nodes/2DGameNode.cpp src/zoe/game/nodes/Node.cpp src/zoe/game/nodes/ScriptNode.cpp src/zoe/game/nodes/SpriteNode.cpp src/zoe/game/nodes/TextureNode.cpp src/zoe/core/XMLParser.cpp src/platform/linux/LinuxInput.cpp src/platform/linux/LinuxWindow.cpp src/platform/OpenGL/OpenGLContext.cpp src/platform/OpenGL/OpenGLIndexBufferImpl.cpp src/platform/OpenGL/OpenGLRenderImpl.cpp src/platform/OpenGL/OpenGLShaderImpl.cpp src/platform/OpenGL/OpenGLTextureImpl.cpp src/platform/OpenGL/OpenGLVertexArrayImpl.cpp src/platform/OpenGL/OpenGLVertexBufferImpl.cpp src/platform/OpenGL/OpenGLVertexBufferLayoutImpl.cpp src/zoe/game/3D/Layer3D.cpp src/zoe/game/3D/Layer3D.h src/zoe/game/3D/Object3D.cpp src/zoe/game/3D/Object3D.h src/zoe/render/Camera.cpp src/zoe/render/Camera.h src/zoe/render/Material.cpp src/zoe/game/3D/Line3D.cpp src/zoe/game/3D/Line3D.h src/zoe/render/Model.cpp src/zoe/render/Model.h src/platform/windows/WindowsInput.cpp src/platform/windows/WindowsWindow.cpp src/zoe/UI/Font.cpp src/zoe/UI/Text.cpp src/zoe/UI/Text.h)

#add library to zoe
list(APPEND ZOE_INCLUDES ${PROJECT_SOURCE_DIR}/dependencies/ChaiScript/include)
target_include_directories(zoe PRIVATE "${ZOE_INCLUDES}")
IF(WIN32)
  add_dependencies(zoe GLEW glew_download)
  add_dependencies(zoe webp)
  add_dependencies(zoe freetype freetype-build)
  target_link_libraries(zoe glfw GLEW opengl32 webp freetype)
ELSE()
  target_link_libraries(zoe pthread glfw GL GLEW webp freetype)
ENDIF()