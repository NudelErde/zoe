/*
 * ellipse.cpp
 *
 *  Created on: 12.11.2019
 *      Author: florian
 */

#include "Ellipse.h"
#include "../render/api/Shader.h"
#include "../render/api/VertexBuffer.h"
#include "../render/api/IndexBuffer.h"
#include "../render/api/VertexArray.h"
#include "../render/api/Render.h"
#include "../Application.h"
#include "../File.h"

namespace Zoe {

static bool hasellipseInit = false;
static Shader* ellipseShader;
static File* ellipseShaderFile;
static VertexBuffer* ellipseVertexBuffer;
static IndexBuffer* ellipseIndexBuffer;
static VertexArray* ellipseVertexArray;
static Render* renderer;
static const char* ellipseShaderSrc =
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

Ellipse::Ellipse(const Rectangle& rect, const Color& color) :
		Component(), rect(rect), color(color) {
	if (!hasellipseInit) {
		hasellipseInit = true;

		ellipseVertexBuffer = Application::getContext().getVertexBuffer();
		ellipseIndexBuffer = Application::getContext().getIndexBuffer();
		ellipseVertexArray = Application::getContext().getVertexArray();
		renderer = Application::getContext().getRender();
		renderer->setAlphaEnabled(true);
		registerVirtualFile("ellipse.shader", ellipseShaderSrc);
		ellipseShaderFile = new File("ellipse.shader");
		ellipseShader = Application::getContext().getShader(*ellipseShaderFile);
		float ellipseVertexData[] { -1, -1, 1, -1, 1, 1, -1, 1 };
		unsigned int ellipseIndexData[] { 0, 1, 2, 2, 3, 0 };
		ellipseVertexBuffer->setData(ellipseVertexData, sizeof(float) * 2 * 4);
		ellipseIndexBuffer->setData(ellipseIndexData, 6);
		VertexBufferLayout* layout = Application::getContext().getVertexBufferLayout();
		layout->push_float(2);
		ellipseVertexArray->set(*ellipseVertexBuffer,*ellipseIndexBuffer, *layout);

		ellipseShader->setUniform4m("Projection",
				Zoe::translate3D(0,0,0)
				* Zoe::scale3D(1/800.0, -1/450.0, 1) * Zoe::translate3D(-800, -450, 0)
		);
	}
}

Ellipse::~Ellipse() {
}

void Ellipse::draw() {
	ellipseShader->setUniform4f("Color", color.r, color.g, color.b, color.a);
	ellipseShader->setUniform4m("ModelView",
			Zoe::translate3D(rect.x+rect.width/2, rect.y+rect.height/2, 0)
					* Zoe::scale3D(rect.width/2, rect.height/2, 1)
					* Zoe::rotateYZ3D(rect.rotation)
	);
	renderer->draw(*ellipseVertexArray, *ellipseShader);
}

}

