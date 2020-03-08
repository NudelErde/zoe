/*
 * Main.cpp
 *
 *  Created on: 17.12.2019
 *      Author: florian
 */

#include <EntryPoint.h>
#include <zoe.h>

#include <math.h>

class MandelLayer : public Zoe::Layer {
public:
    MandelLayer() : Layer("MandelLayer") {
        setSubscribedEvents(Zoe::EventCategoryApplication|Zoe::EventCategoryKeyboard);
    }

    ~MandelLayer() {

    }

    void onAttach() {
        render = Zoe::Application::getContext().getRender();
        indexBuffer = Zoe::Application::getContext().getIndexBuffer();
        vertexBuffer = Zoe::Application::getContext().getVertexBuffer();
        vertexArray = Zoe::Application::getContext().getVertexArray();
        mandelShader = Zoe::Application::getContext().getShader(Zoe::File("shader.glsl"));
        unsigned int indexData[]{0, 1, 2, 2, 3, 0};
        indexBuffer->setData(indexData, 6);
        float vertexData[]{-1, -1, 1, -1, 1, 1, -1, 1};
        vertexBuffer->setData(vertexData, sizeof(float) * 8);
        std::shared_ptr<Zoe::VertexBufferLayout> vertexBufferLayout = Zoe::Application::getContext().getVertexBufferLayout();
        vertexBufferLayout->push_float(2);

        vertexArray->set(*vertexBuffer, *indexBuffer, *vertexBufferLayout);

        rectShader = Zoe::Application::getContext().getShader(Zoe::File("rect.shader"));
    }

    void onDetach() {

    }

    void onEvent(Zoe::Event &event) {
        Zoe::EventDispatcher dispatcher(event);
        static std::function<bool(Zoe::AppRenderEvent &)> onRender = std::bind(&MandelLayer::onRenderEvent, this,
                                                                               std::placeholders::_1);
        static std::function<bool(Zoe::KeyPressedEvent &)> onKey = std::bind(&MandelLayer::onKeyInput, this,
                                                                               std::placeholders::_1);
        dispatcher.dispatch(onRender);
        dispatcher.dispatch(onKey);
    }

    bool onKeyInput(Zoe::KeyPressedEvent& keyEvent){
        Zoe::info(keyEvent.getKeyCode());
    }

    bool onRenderEvent(Zoe::AppRenderEvent &appRenderEvent) {
        if (Zoe::Input::isMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Zoe::vec2 point = Zoe::Input::getMousePosition();
            point.y -= (720 - 640) / 2;
            if (point.x >= 0 && point.x < 640 && point.y >= 0 && point.y < 640) {
                point = point * (1.0/160);
                point.x -= 2;
                point.y -= 2;
                juliaConst = point;
            }
        }

        static float time = 0;
        time += 0.01;

        //juliaConst = {sin(time), cos(time)};
        //juliaConst = juliaConst * (0.5+cos(0.1*time));

        render->setClearColor(0.0f, 0.0f, 0.75f, 0.0f);
        render->clear();

        render->setViewport(0, (720 - 640) / 2, 640, 640);
        Zoe::mat4x4 projection = Zoe::scale3D(1, 1, 1);
        mandelShader->setUniform4m("Projection", projection);
        mandelShader->setUniform1f("maxIterations", 600);
        mandelShader->setUniform2f("juliaConst", 0, 0);
        mandelShader->setUniform1f("mandelJuliaRatio", 1);
        render->draw(*vertexArray, *mandelShader);

        projection = Zoe::translate3D(juliaConst.x/2,-juliaConst.y/2,0)*Zoe::scale3D(0.01,0.01,0.01);
        rectShader->setUniform4m("Projection", projection);
        rectShader->setUniform3f("u_color", 1,1,1);
        render->draw(*vertexArray, *rectShader);

        //render->setViewport(1280 - 640, (720 - 640) / 2, 640, 640);
        //projection = Zoe::scale3D(1, 1, 1);
        //mandelShader->setUniform4m("Projection", projection);
        //mandelShader->setUniform1f("maxIterations", 120);
        //mandelShader->setUniform2f("juliaConst", juliaConst.x, -juliaConst.y);
        //mandelShader->setUniform1f("mandelJuliaRatio", 0);
        //render->draw(*vertexArray, *mandelShader);
    }

private:
    std::shared_ptr<Zoe::Render> render;
    std::shared_ptr<Zoe::IndexBuffer> indexBuffer;
    std::shared_ptr<Zoe::VertexBuffer> vertexBuffer;
    std::shared_ptr<Zoe::Shader> mandelShader;
    std::shared_ptr<Zoe::VertexArray> vertexArray;

    std::shared_ptr<Zoe::Shader> rectShader;
    Zoe::vec2 juliaConst;
};

class App : public Zoe::Application {
public:
    App() {
        getLayerStack().pushLayer(new MandelLayer());
    }

    ~App() override {
    }
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
