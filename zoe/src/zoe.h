/*
 * zoe.h
 *
 *  Created on: 31.03.2019
 *      Author: florian
 */

/**\mainpage Zoe Engine
 * \section intro_sec Introduction
 *
 * Lorem ipsum stuff
 *
 * \section build_sec Building
 * Init github submodules if needed: `git submodule update --init --recursive`\n
 * Make sure your compiler supports C++20\n
 * Make sure you have CMake 3.13+ installed\n
 * \subsection build_win_sec On Windows
 * Install dependencies I don't know what you need. Just try to run cmake and google every error.\n
 * Run `cmake .` to generate the makefiles\n
 * Run `cmake --build .` to compile the default target\n
 * \subsection build_linux_sec On Linux
 * Install dependencies `apt install build-essential cmake libglew-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl-dev`\n
 * Run `cmake .` to generate the makefiles\n
 * Run `cmake --build .` to compile the default target\n
 * \subsection build_mac_sec On MacOS
 * I don't know
 */


#pragma once

#include "zoe/core/Application.h"
#include "zoe/core/Core.h"
#include "zoe/core/File.h"
#include "zoe/core/Input.h"
#include "zoe/core/KeyCode.h"
#include "zoe/core/Layer.h"
#include "zoe/core/LayerStack.h"
#include "zoe/core/MouseCode.h"
#include "zoe/core/VirtualFiles.h"
#include "zoe/core/Window.h"
#include "zoe/core/XMLParser.h"
#include "zoe/core/Scheduler.h"
#include "zoe/core/UTF.h"
#include "zoe/core/Task.h"

#include "zoe/display/Game/Sprite.h"
#include "zoe/display/Game/ModelComponent.h"
#include "zoe/display/Game/WorldObject.h"
#include "zoe/display/Physics/AxisAlignedBox.h"
#include "zoe/display/Physics/PhysicsComponent.h"
#include "zoe/display/Physics/PhysicsGroup.h"
#include "zoe/display/UI/UITool.h"
#include "zoe/display/UI/Button.h"
#include "zoe/display/Camera.h"
#include "zoe/display/Component.h"
#include "zoe/display/ComponentLayer.h"
#include "zoe/display/ChaiScriptComponent.h"
#include "zoe/display/NativeScriptComponent.h"

#include "zoe/event/CommonEvent.h"

#include "zoe/math/mat.h"
#include "zoe/math/vec.h"

#include "zoe/parallel/Thread.h"

#include "zoe/render/api/IndexBuffer.h"
#include "zoe/render/api/Render.h"
#include "zoe/render/api/RenderFlags.h"
#include "zoe/render/api/RenderTarget.h"
#include "zoe/render/api/Shader.h"
#include "zoe/render/api/Texture.h"
#include "zoe/render/api/VertexArray.h"
#include "zoe/render/api/VertexBuffer.h"
#include "zoe/render/api/VertexBufferLayout.h"
#include "zoe/render/Font.h"
#include "zoe/render/GraphicsContext.h"
#include "zoe/render/Material.h"
#include "zoe/render/Model.h"