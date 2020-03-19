//
// Created by florian on 13.03.20.
//

#pragma once

#include "../../Core.h"
#include "../../Layer.h"
#include "Object3D.h"

namespace Zoe {

    class Camera;

    class DLL_PUBLIC Layer3D: public Layer{
    public:
        Layer3D();
        ~Layer3D() override;

        void onEvent(Event& event) override;
        bool renderEvent(AppRenderEvent& event);
        bool tickEvent(AppTickEvent& event);

        void add(const std::shared_ptr<Object3D>& object);

        template<typename T, typename std::enable_if<std::is_base_of<Object3D, T>::value>::type* = nullptr>
        void add(const T& object){add(std::make_shared<T>(object));}

        inline void setCamera(std::shared_ptr<Camera> camera) {this->camera = camera;}
        inline const std::shared_ptr<Camera>& getCamera(){return camera;}

        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onRender();
        virtual void onTick();

    private:
        std::vector<std::shared_ptr<Object3D>> objects;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Render> render;
    };
}



