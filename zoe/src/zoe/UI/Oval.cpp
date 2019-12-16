/*
 * Oval.cpp
 *
 *  Created on: 12.11.2019
 *      Author: florian
 */

#include "Oval.h"
#include "render/api/Shader.h"
#include "render/api/VertexBuffer.h"
#include "render/api/IndexBuffer.h"
#include "render/api/VertexArray.h"
#include "render/api/Render.h"
#include "../Application.h"

namespace Zoe {

static bool hasOvalInit = false;
static Shader* ovalShader;
static File* ovalShaderFile;
static VertexBuffer* ovalVertexBuffer;
static IndexBuffer* ovalIndexBuffer;
static VertexArray* ovalVertexArray;
static Render* renderer;
static const char* ovalShaderSrc =
R"(#shader vertex
#version 130

layout(location=0) in vec2 position;
out vec4 v_color;
out vec2 v_pos;

uniform mat4 Projection;
uniform mat4 ModelView;
uniform vec4 Color;

void main(){
	v_pos = position.xy;
	vec4 pos = vec4(position,0,1);
	gl_Position = Projection * ModelView * pos;
	v_color = Color;
}

#shader fragment
#version 130

in vec4 v_color;
in vec2 v_pos;

out vec4 color;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main(){
	float length = length(v_pos);
	if(length>1)
		discard;
	if(length>0.95)
		color = v_color * vec4(1,1,1,map(length,0.95,1,1,0));
	else	
		color = v_color;
}
)";

Oval::Oval(const Rectangle& rect, const Color& color) :
		Component(), rect(rect), color(color) {
	if (!hasOvalInit) {
		hasOvalInit = true;

		ovalVertexBuffer = Application::getContext().getVertexBuffer();
		ovalIndexBuffer = Application::getContext().getIndexBuffer();
		ovalVertexArray = Application::getContext().getVertexArray();
		renderer = Application::getContext().getRender();
		renderer->setAlphaEnabled(true);
		registerVirtualFile("oval.shader", ovalShaderSrc);
		ovalShaderFile = new File("oval.shader");
		ovalShader = Application::getContext().getShader(*ovalShaderFile);
		float ovalVertexData[] { -1, -1, 1, -1, 1, 1, -1, 1 };
		unsigned int ovalIndexData[] { 0, 1, 2, 2, 3, 0 };
		ovalVertexBuffer->setData(ovalVertexData, sizeof(float) * 2 * 4);
		ovalIndexBuffer->setData(ovalIndexData, 6);
		VertexBufferLayout* layout = Application::getContext().getVertexBufferLayout();
		layout->push_float(2);
		ovalVertexArray->set(*ovalVertexBuffer,*ovalIndexBuffer, *layout);

		ovalShader->setUniform4m("Projection",
				Zoe::translate3D(0,0,0)
				* Zoe::scale3D(1/800.0, -1/450.0, 1) * Zoe::translate3D(-800, -450, 0)
		);
	}
}

Oval::~Oval() {
}

void Oval::draw() {
	ovalShader->setUniform4f("Color", color.r, color.g, color.b, color.a);
	ovalShader->setUniform4m("ModelView",
			Zoe::translate3D(rect.x, rect.y, 0)
					* Zoe::scale3D(rect.width/2, rect.height/2, 1)
					* Zoe::rotateYZ3D(rect.rotation)
	);
	renderer->draw(*ovalVertexArray, *ovalShader);
}

}

