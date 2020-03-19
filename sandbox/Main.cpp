//
// Created by florian on 19.03.20.
//

#include "zoe.h"
#include "EntryPoint.h"

class Layer : public Zoe::Layer3D {
public:

    void onAttach() override {

        Zoe::Line3D x = Zoe::Line3D({0, 0, 0}, {10000, 0, 0});
        x.setColor({1, 0, 0, 1}, {1, 0, 0, 1});
        Zoe::Line3D y = Zoe::Line3D({0, 0, 0}, {0, 10000, 0});
        y.setColor({0, 1, 0, 1}, {0, 1, 0, 1});
        Zoe::Line3D z = Zoe::Line3D({0, 0, 0}, {0, 0, 10000});
        z.setColor({0, 0, 1, 1}, {0, 0, 1, 1});

        Zoe::Line3D mx = Zoe::Line3D({0, 0, 0}, {-10000, 0, 0});
        mx.setColor({1, 0, 0, 0.5}, {1, 0, 0, 0.5});
        Zoe::Line3D my = Zoe::Line3D({0, 0, 0}, {0, -10000, 0});
        my.setColor({0, 1, 0, 0.5}, {0, 1, 0, 0.5});
        Zoe::Line3D mz = Zoe::Line3D({0, 0, 0}, {0, 0, -10000});
        mz.setColor({0, 0, 1, 0.5}, {0, 0, 1, 0.5});

        add(x);
        add(y);
        add(z);
        add(mx);
        add(my);
        add(mz);

        setDebugCamera(true);
    }

    void onDetach() override {

    }

    void onTick() override {
        Zoe::vec3 go = {1,2,3};
        Zoe::vec3 gr = {-4,2,3};

        Zoe::vec3 ho = {-2,2,2};
        Zoe::vec3 hr = {1,-3,5};
        Zoe::vec3 ze = {0,0,0};

        Zoe::vec3 normal = gr.crossProduct(hr).normalize()*10;

        if (!(loaded & BIT(0)) && Zoe::Input::isKeyPressed(KEY_1)) {
            loaded |= BIT(0);
            add(Zoe::Line3D(ze, go, {0.75, 0, 0.75, 1}, {0.75, 0, 0.75, 1}));
        }else if (!(loaded & BIT(1)) && Zoe::Input::isKeyPressed(KEY_2)) {
            loaded |= BIT(1);
            add(Zoe::Line3D(go+gr*-20, go+gr*20, {0.75, 0, 0.75, 0.5}, {0.75, 0, 0.75, 0.5}));
        }else if (!(loaded & BIT(2)) && Zoe::Input::isKeyPressed(KEY_3)) {
            loaded |= BIT(2);
            add(Zoe::Line3D(ze, ho, {0, 0.75, 0.75, 1}, {0, 0.75, 0.75, 1}));
            add(Zoe::Line3D(ho+hr*-20, ho+hr*20, {0, 0.75, 0.75, 0.5}, {0, 0.75, 0.75, 0.5}));
        }else if (!(loaded & BIT(3)) && Zoe::Input::isKeyPressed(KEY_4)) {
            loaded |= BIT(3);
            add(Zoe::Line3D(ze, normal, {0.75, 0.75, 0, 1}, {0.75, 0.75, 0, 1}));
        }
    }

    void onRender() override {

    }

    int loaded = 0;

};

class App : public Zoe::Application {
public:
    App() {
        getLayerStack().pushLayer(new Layer());
    }

    ~App() override = default;
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
