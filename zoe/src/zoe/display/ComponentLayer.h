//
// Created by Florian on 15.07.2020.
//

#pragma once

#include "../core/Core.h"
#include "../core/Layer.h"
#include "../core/File.h"

#include "Component.h"

namespace Zoe{

class DLL_PUBLIC ComponentLayer: public Layer, public BaseComponent {
public:
    ComponentLayer();
    ComponentLayer(const unsigned int& width, const unsigned int& height);
    ~ComponentLayer() override;

    void onAttach() override;
    void onDetach() override;
    void onEvent(Event& event) final;

    void load(const File& file);
    void load(const XMLNode& node);

    inline const std::shared_ptr<Camera>& getCamera() {return camera;}

    inline void setCamera(const std::shared_ptr<Camera>& cam) {camera = cam;}
    static bool isKeyPressed(int keycode);
    static bool isMouseButtonPressed(int button);

    static vec2 getMousePosition();

protected:
    void fill(const XMLNode &node) override;
    void postFill() override;
    void onUpdate(double time) override;
    void onDraw(const Camera &camera) override;
    void onInputEvent(Event &event) override;
private:
    void onDrawEvent(AppRenderEvent& );
    void onTickEvent(AppTickEvent& );

private:
    std::chrono::time_point<std::chrono::steady_clock> timeOfLastTick;

    std::shared_ptr<Camera> camera;

    std::shared_ptr<RenderTarget> target;
    std::shared_ptr<Render> render;
    std::shared_ptr<Render> displayRender;

    unsigned int width, height;
};

}