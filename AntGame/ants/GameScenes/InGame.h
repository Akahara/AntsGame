#pragma once

#include "src/Sandbox/Scene.h"

class InGame : public Scene {


private:

public:

	void step(float delta) override {

		try {
			std::cout << "yo \n" << std::endl;
		}
		catch (...) {


		}
		
	}



};