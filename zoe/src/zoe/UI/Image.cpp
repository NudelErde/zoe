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
#include "../render/GraphicsContext.h"
#include "../Console.h"

namespace Zoe {

    struct imageData {
        std::shared_ptr<Shader> shader;
        std::shared_ptr<File> file;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;
        std::shared_ptr<VertexArray> vertexArray;
        std::shared_ptr<Render> renderer;
    };

    static imageData data;

    static bool hasImageInit = false;
    static const char *imageShaderSrc =
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

    Image::Image(const Rect &rect, const File &file) : rect(rect) {
        texture = Application::getContext().getTexture(file);
        if (!hasImageInit) {
            hasImageInit = true;

            data.vertexBuffer = Application::getContext().getVertexBuffer();
            data.indexBuffer = Application::getContext().getIndexBuffer();
            data.vertexArray = Application::getContext().getVertexArray();
            data.renderer = Application::getContext().getRender();
            data.renderer->setAlphaEnabled(true);
            registerVirtualFile("image.shader", imageShaderSrc);
            data.file = std::make_shared<File>("image.shader");
            data.shader = Application::getContext().getShader(*data.file);
            float imageVertexData[]{0, 0, 1, 0, 1, 1, 0, 1};
            unsigned int imageIndexData[]{0, 1, 2, 2, 3, 0};
            data.vertexBuffer->setData(imageVertexData, sizeof(float) * 8);
            data.indexBuffer->setData(imageIndexData, 6);
            std::shared_ptr<VertexBufferLayout> layout = Application::getContext().getVertexBufferLayout();
            layout->push_float(2);
            data.vertexArray->set(*data.vertexBuffer, *data.indexBuffer, *layout);

            data.shader->setUniform4m("Projection", Zoe::translate3D(0, 0, 0)
                                                    * Zoe::scale3D(1 / 800.0, -1 / 450.0, 1) *
                                                    Zoe::translate3D(-800, -450, 0));
        }
    }

    Image::~Image() = default;

    void Image::draw() {
        data.shader->setTexture("tex", *texture);
        data.shader->setUniform4m("ModelView",
                                  Zoe::translate3D(rect.x, rect.y, 0)
                                  * Zoe::scale3D(rect.width, rect.height, 1)
                                  * Zoe::rotateYZ3D(rect.rotation)
        );
        data.renderer->draw(*data.vertexArray, *data.shader);
    }

}
