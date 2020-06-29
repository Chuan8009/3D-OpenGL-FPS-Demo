#ifndef ENGINE_H
#define ENGINE_H

#include "Environment.h"

#include "../src/Resources/Program.h"
#include "../src/Resources/Model.h"
#include "../src/Resources/ModelLoader.h"

class Entity;

class Engine {
public:
	Engine();
	~Engine();

	void run();
	void input();
	void render();

	void debug_draw();
private:
	bool _exit;

	void make_lights();
	void spawn_enemies();

	Environment _environment;
};

#endif