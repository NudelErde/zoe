/*
 * TextureNode.cpp
 *
 *  Created on: 26.12.2019
 *      Author: florian
 */

#include "TextureNode.h"
#include "../../Application.h"
#include "../../Console.h"
#include "../../render/GraphicsContext.h"

namespace Zoe{

struct textureNodeData{
	std::shared_ptr<Render> renderer;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;
	std::shared_ptr<VertexArray> vertexArray;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<File> file;
};

static textureNodeData data;
static bool textureInit = false;
static const char* textureShaderSrc =
R"(#shader vertex
#version 130

layout(location=0) in vec2 position;
out vec2 v_pos;

uniform mat4 ModelViewProjection;

void main(){
	gl_Position = ModelViewProjection * vec4(position,0,1);
	v_pos = position.xy;
}

#shader fragment
#version 130

in vec2 v_pos;

out vec4 color;

uniform sampler2D tex;

void main(){
	color = texture2D(tex,v_pos);
}
)";

//---------------------------------------------------------------------

TextureNode::TextureNode():width(-1),height(-1) {
	if(!textureInit){
		textureInit = true;
		data.vertexBuffer = Application::getContext().getVertexBuffer();
		data.indexBuffer = Application::getContext().getIndexBuffer();
		data.vertexArray = Application::getContext().getVertexArray();
		data.renderer = Application::getContext().getRender();
		data.renderer->setAlphaEnabled(true);
		registerVirtualFile("textureNode.shader", textureShaderSrc);
		data.file = std::make_shared<File>("textureNode.shader");
		data.shader = Application::getContext().getShader(*data.file);
		float textureVertexData[] {0,0,1,0,1,1,0,1};
		unsigned int textureIndexData[] {0,1,2,2,3,0};
		data.vertexBuffer->setData(textureVertexData, sizeof(float)*8);
		data.indexBuffer->setData(textureIndexData, 6);
		std::shared_ptr<VertexBufferLayout> layout = Application::getContext().getVertexBufferLayout();
		layout->push_float(2);
		data.vertexArray->set(*data.vertexBuffer, *data.indexBuffer, *layout);
	}
}

TextureNode::~TextureNode() {
}

void TextureNode::tick(double delta) {
}

void TextureNode::draw(mat4x4 mat) {
	mat = mat * translate3D(pos.x, pos.y, pos.z) * scale3D(width, height, 1);
	data.shader->setTexture("tex", *texture);
	data.shader->setUniform4m("ModelViewProjection", mat);
	data.renderer->draw(*data.vertexArray, *data.shader);
}

void TextureNode::init(XMLNode& node) {
	std::string value = "";
	if (!(value = node.attributes["x"]).empty()) {
		pos.x = fromString<float>(value);
	}
	if (!(value = node.attributes["y"]).empty()) {
		pos.y = fromString<float>(value);
	}
	if (!(value = node.attributes["z"]).empty()) {
		pos.z = fromString<float>(value);
	}
	if (!(value = node.attributes["width"]).empty()) {
		width = fromString<float>(value);
	}
	if (!(value = node.attributes["height"]).empty()) {
		height = fromString<float>(value);
	}
	if (!(value = node.attributes["src"]).empty()) {
		texture = Application::getContext().getTexture(File(value));
	}
}

void TextureNode::setByKey(std::string key, std::string value) {
}

void TextureNode::setByKey(std::string key, double value) {
	if(key == "x"){
		pos.x = value;
	}else if(key == "y"){
		pos.y = value;
	}else if(key == "z"){
		pos.z = value;
	}else if(key == "width"){
		width = value;
	}else if(key == "height"){
		height = value;
	}
}

std::string TextureNode::getStringByKey(std::string key) {
	return "";
}

double TextureNode::getDoubleByKey(std::string key) {
	if(key == "x"){
		return pos.x;
	}else if(key == "y"){
		return pos.y;
	}else if(key == "z"){
		return pos.z;
	}else if(key == "width"){
		return width;
	}else if(key == "height"){
		return height;
	}
	return 0;
}

}
