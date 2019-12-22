/*
 * Image.cpp
 *
 *  Created on: 17.12.2019
 *      Author: florian
 */

#include "Image.h"
#include "../Application.h"
#include "../render/api/Shader.h"
#include "../render/api/IndexBuffer.h"
#include "../render/api/Render.h"
#include "../render/api/Texture.h"
#include "../render/api/VertexArray.h"
#include "../render/api/VertexBuffer.h"
#include "../render/api/VertexBufferLayout.h"

namespace Zoe{

static bool hasImageInit = false;
static Shader* imageShader;
static File* imageShaderFile;
static VertexBuffer* imageVertexBuffer;
static IndexBuffer* imageIndexBuffer;
static VertexArray* imageVertexArray;
static Render* renderer;

static const char* imageShaderSrc =
R"(#shader vertex
#version 130

layout(location=0) in vec2 position;
out vec2 v_pos;

uniform mat4 Projection;
uniform mat4 ModelView;

void main(){
	gl_Position = Projection * ModelView * vec4(position,0,1);
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

Image::Image(const Rectangle& rect, const File& file): rect(rect){
	texture = Application::getContext().getTexture(file);
	if(!hasImageInit){
		hasImageInit = true;

		imageVertexBuffer = Application::getContext().getVertexBuffer();
		imageIndexBuffer = Application::getContext().getIndexBuffer();
		imageVertexArray = Application::getContext().getVertexArray();
		renderer = Application::getContext().getRender();
		renderer->setAlphaEnabled(true);
		registerVirtualFile("image.shader", imageShaderSrc);
		imageShaderFile = new File("image.shader");
		imageShader = Application::getContext().getShader(*imageShaderFile);
		float imageVertexData[] {0,0,1,0,1,1,0,1};
		unsigned int imageIndexData[] {0,1,2,2,3,0};
		imageVertexBuffer->setData(imageVertexData, sizeof(float)*8);
		imageIndexBuffer->setData(imageIndexData, 6);
		VertexBufferLayout* layout = Application::getContext().getVertexBufferLayout();
		layout->push_float(2);
		imageVertexArray->set(*imageVertexBuffer, *imageIndexBuffer, *layout);

		imageShader->setUniform4m("Projection", Zoe::translate3D(0,0,0)
				* Zoe::scale3D(1/800.0, -1/450.0, 1) * Zoe::translate3D(-800, -450, 0));
	}
}

Image::~Image(){
}

void Image::draw(){
	imageShader->setTexture("tex", *texture);
	imageShader->setUniform4m("ModelView",
			Zoe::translate3D(rect.x, rect.y, 0)
					* Zoe::scale3D(rect.width, rect.height, 1)
					* Zoe::rotateYZ3D(rect.rotation)
	);
	renderer->draw(*imageVertexArray, *imageShader);
}

}
