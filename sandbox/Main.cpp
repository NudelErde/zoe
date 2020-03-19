/*
 * Main.cpp
 *
 *  Created on: 17.12.2019
 *      Author: florian
 */

#include <EntryPoint.h>
#include <zoe.h>

class Layer: public Zoe::Layer3D{
public:

    void onAttach() override{
        Zoe::Line3D x = Zoe::Line3D({0,0,0},{10000,0,0});
        x.setColor({1,0,0,1},{1,0,0,1});
        Zoe::Line3D y = Zoe::Line3D({0,0,0},{0,10000,0});
        y.setColor({0,1,0,1},{0,1,0,1});
        Zoe::Line3D z = Zoe::Line3D({0,0,0},{0,0,10000});
        z.setColor({0,0,1,1},{0,0,1,1});

        Zoe::Line3D mx = Zoe::Line3D({0,0,0},{-10000,0,0});
        mx.setColor({1,0,0,0.5},{1,0,0,0.5});
        Zoe::Line3D my = Zoe::Line3D({0,0,0},{0,-10000,0});
        my.setColor({0,1,0,0.5},{0,1,0,0.5});
        Zoe::Line3D mz = Zoe::Line3D({0,0,0},{0,0,-10000});
        mz.setColor({0,0,1,0.5},{0,0,1,0.5});

        add(x);
        add(y);
        add(z);
        add(mx);
        add(my);
        add(mz);

        cam = std::make_shared<Zoe::Camera>();
        setCamera(cam);
        cam->setProjectionMatrix(Zoe::perspective(1,100,70,16.0f/9.0f));
        cam->setPosition({0,0, 10});
        cam->setRotation(0,0,0);
    }

    void onTick() override{
        if(Zoe::Input::isKeyPressed(KEY_SPACE)){
            cam->setPosition(cam->getPosition()+(Zoe::vec3){0,0.1,0});
        }else if(Zoe::Input::isKeyPressed(KEY_LEFT_SHIFT)){
            cam->setPosition(cam->getPosition()+(Zoe::vec3){0,-0.1,0});
        }else if(Zoe::Input::isKeyPressed(KEY_W)){
            cam->setPosition(cam->getPosition()+cam->getDirection()*0.1);
        }else if(Zoe::Input::isKeyPressed(KEY_S)){
            cam->setPosition(cam->getPosition()+cam->getDirection()*-0.1);
        }else if(Zoe::Input::isKeyPressed(KEY_D)){
            cam->setPosition(cam->getPosition()+cam->getDirection().crossProduct({0,0.1,0}));
        }else if(Zoe::Input::isKeyPressed(KEY_A)){
            cam->setPosition(cam->getPosition()+cam->getDirection().crossProduct({0,-0.1,0}));
        }else if(Zoe::Input::isKeyPressed(KEY_RIGHT)){
            cam->setYaw(cam->getRotation().y-0.01f);
        }else if(Zoe::Input::isKeyPressed(KEY_LEFT)){
            cam->setYaw(cam->getRotation().y+0.01f);
        }else if(Zoe::Input::isKeyPressed(KEY_UP)){
            cam->setPitch(cam->getRotation().x+0.01f);
        }else if(Zoe::Input::isKeyPressed(KEY_DOWN)){
            cam->setPitch(cam->getRotation().x-0.01f);
        }else if(Zoe::Input::isKeyPressed(KEY_E)){
            cam->setRoll(cam->getRotation().z-0.01f);
        }else if(Zoe::Input::isKeyPressed(KEY_Q)){
            cam->setRoll(cam->getRotation().z+0.01f);
        }
    }

    void onRender() override{
    }

private:
    std::shared_ptr<Zoe::Camera> cam;
};

class App: public Zoe::Application{
public:
	App(){
		//getLayerStack().pushLayer(new Zoe::Layer2D(Zoe::File("game.xml")));
		getLayerStack().pushLayer(new Layer());
	}
	~App(){
	}
};

Zoe::Application* Zoe::createApplication(){
	return new App();
}
