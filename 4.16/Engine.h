#ifndef ENGINE_H
#define ENGINE_H

#include "Environment.h"

#include "Program.h"
#include "Model.h"
#include "ModelLoader.h"

class Engine {
public:
	Engine();
	~Engine();

	void run();
	void input();
	void render();
private:
	bool _exit;

	Model *_model;

	Environment _environment;
};

#endif