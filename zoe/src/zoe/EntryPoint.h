/*
 * EntryPoint.h
 *
 *  Created on: 31.03.2019
 *      Author: florian
 */

#pragma once

#include "Application.h"

extern Zoe::Application* Zoe::createApplication();

int main(){
	auto app = Zoe::createApplication();
	app->run();
	delete app;
}
