//
// Created by florian on 19.03.20.
//

#include "Line3D.h"
#include "../../Application.h"

namespace Zoe {

#pragma pack(push)
#pragma pack(1)
    struct LineVertex {
        LineVertex(vec3 pos, vec4 color) : pos(pos), color(color) {}

        vec3 pos;
        vec4 color;
    };
#pragma pack(pop)

    static bool lineInit = false;
    static std::shared_ptr<IndexBuffer> indexBuffer;
    std::shared_ptr<VertexBufferLayout> layout;
    static mat4x4 modelMatrix;
    static std::shared_ptr<Render> renderer;
    static const char *shaderSource =
            R"(
#tag modelMatrix model
#tag viewMatrix view
#tag projectionMatrix projection
#shader vertex
#version 130

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;

out vec4 v_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	vec4 pos = vec4(position,1);
	gl_Position = projection * view * model * pos;
	v_color = color;
}

#shader fragment
#version 130

in vec4 v_color;

out vec4 color;

void main(){
    color = v_color;
}

)";

    Line3D::Line3D(const vec3 &start, const vec3 &end)
            : Object3D(), start(start), end(end), colorStart({0, 0, 0, 1}), colorEnd({1, 1, 1, 1}) {
        if (!lineInit) {
            lineInit = true;
            modelMatrix = translate3D(0, 0, 0);
            registerVirtualFile("lineShader.shader", shaderSource);
            layout = Application::getContext().getVertexBufferLayout();
            layout->push_float(3); //position
            layout->push_float(4); //color
            indexBuffer = Application::getContext().getIndexBuffer();
            unsigned int indexData[]{0, 1, 2, 2, 3, 0};
            indexBuffer->setData(indexData, 6);
            renderer = Application::getContext().getRender();
            renderer->setAlphaEnabled(true);
        }
        File shaderFile("lineShader.shader");
        setMaterial(Material(shaderFile, layout));
        vertexBuffer = Application::getContext().getVertexBuffer(true);
        LineVertex emptyVertexData[4] = {
                LineVertex({0, 0, 0}, {1, 1, 1, 1}),
                LineVertex({0, 0, 0}, {1, 1, 1, 1}),
                LineVertex({0, 0, 0}, {1, 1, 1, 1}),
                LineVertex({0, 0, 0}, {1, 1, 1, 1})
        };
        float* arr = reinterpret_cast<float*>(emptyVertexData);
        vertexBuffer->setData(emptyVertexData, sizeof(LineVertex) * 4);
        lineModel = Model(vertexBuffer, indexBuffer, layout);
    }

    Line3D::~Line3D() = default;

    void Line3D::draw(Camera &camera) {
        vec3 lineDirection = end - start;
        vec3 startNormal = (camera.getPosition()-start).crossProduct(lineDirection).normalize();
        vec3 endNormal = (camera.getPosition()-end).crossProduct(lineDirection).normalize();

        LineVertex vertices[4]{
                LineVertex(start + 0.5*startNormal, colorStart),
                LineVertex(start - 0.5*startNormal, colorStart),
                LineVertex(end - 0.5*endNormal, colorEnd),
                LineVertex(end + 0.5*endNormal, colorEnd)
        };
        vertexBuffer->setData(vertices, sizeof(LineVertex) * 4);
        camera.draw(material, lineModel, renderer);
    }

    void Line3D::setPosition(const vec3 &startPosition, const vec3 &endPosition) {
        this->start = startPosition;
        this->end = endPosition;
    }

    void Line3D::setColor(const vec4& startColor, const vec4& endColor) {
        colorStart = startColor;
        colorEnd = endColor;
    }

}