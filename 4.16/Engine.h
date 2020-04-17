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

	static constexpr Program _program[] = { { GL_VERTEX_SHADER, "Data\\Shaders\\color_shader.vert"   },
											 { GL_FRAGMENT_SHADER, "Data\\Shaders\\color_shader.frag" },
											 { GL_NONE, 0 }
	};

	GLuint _program_id;

	Model *_model;

	Environment _environment;
};

#endif