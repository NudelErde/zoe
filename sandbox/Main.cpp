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
	}
	~TestLayer(){
	}

	void onAttach(){
		Zoe::Color red = {1,0,0,1};
		setColor(red);
		addImage(0,0,300,300,Zoe::File("5264345_460swp.webp"));
		addImage(300,0,100,100,Zoe::File("5264345_460swp.webp"));
		addEllipse(0, 0, 250, 250);
		Zoe::XMLNode node = Zoe::XMLParser::read(Zoe::File("test.xml"));
		Zoe::info("name: ",node.name);
		Zoe::info("attribute[stuff]: ", node.attributes["stuff"]);
		Zoe::info("content: ", node.content);
		Zoe::info("children[0].name: ", node.children[0].name);
		Zoe::info("children[0].content: ", node.children[0].content);
		Zoe::info("children[1].name: ", node.children[1].name);
		Zoe::info("children[1].content: ", node.children[1].content);
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
