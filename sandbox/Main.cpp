/*
 * Main.cpp
 *
 *  Created on: 17.12.2019
 *      Author: florian
 */

#include <EntryPoint.h>
#include <zoe.h>

class App: public Zoe::Application{
public:
	App(){
		getLayerStack().pushLayer(new Zoe::Layer2D(Zoe::File("game.xml")));
	}
	~App(){
	}
};

Zoe::Application* Zoe::createApplication(){
	return new App();
}
