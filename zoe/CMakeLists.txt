#add Zoe as shared library
add_library(zoe SHARED src/zoe/Sort.cpp src/zoe/Application.cpp src/zoe/Console.cpp src/zoe/File.cpp src/zoe/KeyCode.cpp src/zoe/Layer.cpp src/zoe/LayerStack.cpp src/zoe/UI/UILayer.cpp src/zoe/UI/Ellipse.cpp src/zoe/UI/CommonUI.cpp src/zoe/UI/Image.cpp src/zoe/render/Model.cpp src/zoe/parallel/Thread.cpp src/zoe/parallel/Parallel.cpp src/zoe/math/mat.cpp src/zoe/math/vec.cpp src/zoe/game/nodes/2DGameNode.cpp src/zoe/game/nodes/Node.cpp src/zoe/game/nodes/ScriptNode.cpp src/zoe/game/nodes/SpriteNode.cpp src/zoe/game/nodes/TextureNode.cpp src/zoe/core/XMLParser.cpp src/platform/linux/LinuxInput.cpp src/platform/linux/LinuxWindow.cpp src/platform/OpenGL/OpenGLContext.cpp src/platform/OpenGL/OpenGLIndexBufferImpl.cpp src/platform/OpenGL/OpenGLRenderImpl.cpp src/platform/OpenGL/OpenGLShaderImpl.cpp src/platform/OpenGL/OpenGLTextureImpl.cpp src/platform/OpenGL/OpenGLVertexArrayImpl.cpp src/platform/OpenGL/OpenGLVertexBufferImpl.cpp src/platform/OpenGL/OpenGLVertexBufferLayoutImpl.cpp)

#create glfw files
message("Create glfw files")
execute_process(COMMAND ${CMAKE_COMMAND} -DBUILD_SHARED_LIBS=off ${PROJECT_SOURCE_DIR}/dependencies/glfw)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_SOURCE_DIR}/dependencies/glfw)
message("Done creating glfw files")

#make glfw library
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/dependencies/glfw/src/libglfw3.a"
        INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/dependencies/glfw/include")

#include ChaiScriptSource
target_include_directories(zoe PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/ChaiScript/include)

#create webp
message("Create webp files")
execute_process(COMMAND ${CMAKE_COMMAND} -Wno-dev ${PROJECT_SOURCE_DIR}/dependencies/webp)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_SOURCE_DIR}/dependencies/webp)
message("Done creating webp files")

#make webp library
add_library(webp STATIC IMPORTED)
set_target_properties(webp PROPERTIES
        IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/dependencies/webp/libwebp.a"
        INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/dependencies/webp/src")


#add library to zoe
target_link_libraries(zoe pthread glfw GL GLEW webp)