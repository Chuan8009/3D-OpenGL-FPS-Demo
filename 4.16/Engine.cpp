#include "Engine.h"

#include "Clock.h"
#include "Window.h"

#include <iostream>

Engine::Engine() :
	_exit ( false )
{
	Clock* clock = new Clock;
	_environment.set_clock(clock);

	Window* window = new Window;
	_environment.set_window(window);
}

Engine::~Engine() {
	_environment.shut_down();
}

void Engine::run() {

	while (!_exit) {
		render();

		_environment.get_window()->update();

		input();

		_environment.get_clock()->update();
	}
	
}

void Engine::input() {
	glfwPollEvents();

	_exit = glfwWindowShouldClose(_environment.get_window()->get_glfw_window()) || glfwGetKey(_environment.get_window()->get_glfw_window(), GLFW_KEY_ESCAPE);
}

void Engine::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, _environment.get_window()->color);

	glfwSwapBuffers(_environment.get_window()->get_glfw_window());
}