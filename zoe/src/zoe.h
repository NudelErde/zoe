/*
 * zoe.h
 *
 *  Created on: 31.03.2019
 *      Author: florian
 */

#pragma once

//---STD-HEADERS---------------------------
#include <stdio.h>

//---GLFW-HEADER---------------------------

#include <GLFW/glfw3.h>

//---ZOE-HEADERS---------------------------
#include "zoe/Console.h"
#include "zoe/Application.h"
#include "zoe/Core.h"
#include "zoe/event/Event.h"
#include "zoe/event/ApplicationEvent.h"
#include "zoe/event/MouseEvent.h"
#include "zoe/event/KeyEvent.h"
#include "zoe/event/WindowEvent.h"
#include "zoe/Window.h"
#include "zoe/Layer.h"
#include "zoe/LayerStack.h"
#include "zoe/Input.h"
#include "zoe/render/api/IndexBuffer.h"
#include "zoe/render/api/VertexBuffer.h"
#include "zoe/render/api/Shader.h"
#include "zoe/render/api/VertexArray.h"
#include "zoe/render/api/VertexBufferLayout.h"
#include "zoe/render/api/Render.h"
#include "zoe/math/vec.h"
#include "zoe/KeyCode.h"
#include "zoe/MouseCode.h"
#include "zoe/math/mat.h"
#include "zoe/Sort.h"
#include "zoe/parallel/Thread.h"
#include "zoe/UI/UILayer.h"
#include "zoe/UI/CommonUI.h"
#include "zoe/UI/Ellipse.h"
#include "zoe/File.h"
#include "zoe/core/XMLParser.h"
#include "zoe/UI/Image.h"
#include "zoe/event/CommonEvent.h"
#include "zoe/game/nodes/2DGameNode.h"
#include "zoe/game/nodes/Node.h"
#include "zoe/game/nodes/PhysicNode.h"
#include "zoe/game/nodes/ScriptNode.h"
#include "zoe/game/nodes/SpriteNode.h"
#include "zoe/game/nodes/TextureNode.h"
#include "zoe/game/nodes/TileMapNode.h"
