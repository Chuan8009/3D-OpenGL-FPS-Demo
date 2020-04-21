#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "Engine.h"

int main() {
	glfwInit();

	Engine engine;
	engine.run();

	return 0;
}