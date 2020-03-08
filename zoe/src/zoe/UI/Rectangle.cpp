//
// Created by florian on 12.02.20.
//

#include <memory>
#include "Rectangle.h"
#include "../render/api/Render.h"
#include "../Application.h"

namespace Zoe {

    struct rectangleData {
        std::shared_ptr<Shader> shader;
        std::shared_ptr<File> file;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;
        std::shared_ptr<VertexArray> vertexArray;
        std::shared_ptr<Render> renderer;
    };

    static rectangleData data;

    static bool hasRectangleInit = false;
    static const char *rectangleShaderSrc =
            R"(#shader vertex
#version 130

layout(location=0) in vec2 position;
out vec4 v_color;

uniform mat4 Projection;
uniform mat4 ModelView;
uniform vec4 Color;

void main(){
	vec4 pos = vec4(position,0,1);
	gl_Position = Projection * ModelView * pos;
	v_color = Color;
}

#shader fragment
#version 130

in vec4 v_color;

out vec4 color;

void main(){
    color = v_color;
}
)";

    Rectangle::Rectangle(const Zoe::Rect &rect, const Zoe::Color &color) : Component(), rect(rect), color(color) {
        if (!hasRectangleInit) {
            hasRectangleInit = true;

            data.vertexBuffer = Application::getContext().getVertexBuffer();
            data.indexBuffer = Application::getContext().getIndexBuffer();
            data.vertexArray = Application::getContext().getVertexArray();
            data.renderer = Application::getContext().getRender();
            data.renderer->setAlphaEnabled(true);
            registerVirtualFile("rectangle.shader", rectangleShaderSrc);
            data.file = std::make_shared<File>("rectangle.shader");
            data.shader = Application::getContext().getShader(*data.file);
            float ellipseVertexData[]{-1, -1, 1, -1, 1, 1, -1, 1};
            unsigned int ellipseIndexData[]{0, 1, 2, 2, 3, 0};
            data.vertexBuffer->setData(ellipseVertexData, sizeof(float) * 2 * 4);
            data.indexBuffer->setData(ellipseIndexData, 6);
            std::shared_ptr<VertexBufferLayout> layout = Application::getContext().getVertexBufferLayout();
            layout->push_float(2);
            data.vertexArray->set(*data.vertexBuffer, *data.indexBuffer, *layout);

            data.shader->setUniform4m("Projection",
                                      Zoe::translate3D(0, 0, 0)
                                      * Zoe::scale3D(1 / 800.0, -1 / 450.0, 1) * Zoe::translate3D(-800, -450, 0)
            );
        }
    }

    Rectangle::~Rectangle() = default;

    void Rectangle::draw() {
        data.shader->setUniform4f("Color", color.r, color.g, color.b, color.a);
        data.shader->setUniform4m("ModelView",
                                  Zoe::translate3D(rect.x + rect.width / 2, rect.y + rect.height / 2, 0)
                                  * Zoe::scale3D(rect.width / 2, rect.height / 2, 1)
                                  * Zoe::rotateYZ3D(rect.rotation)
        );
        data.renderer->draw(*data.vertexArray, *data.shader);
    }

}
