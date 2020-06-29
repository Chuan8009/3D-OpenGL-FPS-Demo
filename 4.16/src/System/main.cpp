#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <Windows.h>

#include "Engine.h"

int main() {

	HWND console = GetConsoleWindow();
	SetWindowPos(console, NULL, -1000, 100, 1000, 500, NULL);

	glfwInit();

	Engine engine;
	engine.run();

	return 0;
}