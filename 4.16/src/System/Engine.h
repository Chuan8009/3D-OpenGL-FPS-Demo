#ifndef ENGINE_H
#define ENGINE_H

#include "Environment.h"

#include "../src/Resources/Program.h"
#include "../src/Resources/Model.h"
#include "../src/Resources/ModelLoader.h"

class Engine {
public:
	Engine();
	~Engine();

	void run();
	void input();
	void render();
private:
	bool _exit;

	Environment _environment;
};

#endif