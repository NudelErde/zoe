/*
 * Main.cpp
 *
 *  Created on: 17.12.2019
 *      Author: florian
 */

#include <EntryPoint.h>
#include <zoe.h>

class TestLayer: public Zoe::UILayer{
public:
	TestLayer(){
		Zoe::Color red = {1,0,0,1};
		setColor(red);
		addImage(0,0,300,300,Zoe::File("5264345_460swp.webp"));
		addImage(300,0,100,100,Zoe::File("5264345_460swp.webp"));
		addOval(0, 0, 250, 250);
	}
	~TestLayer(){
	}

	void onAttach(){
	}
	void onDetach(){
	}
};

class App: public Zoe::Application{
public:
	App(){
		getLayerStack().pushOverlay(new TestLayer());
	}
	~App(){
	}
};

Zoe::Application* Zoe::createApplication(){
	return new App();
}
