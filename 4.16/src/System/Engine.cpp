#include "Engine.h"

#include "../src/Utility/Clock.h"
#include "../src/Resources/Window.h"
#include "../src/Resources/ResourceManager.h"

#include "../src/Resources/Camera.h"

#include "../src/Resources/Entities/Entity.h"
#include "../src/Resources/Entities/Components/Component.h"
#include "../src/Resources/Entities/Components/PlayerComponent.h"
#include "../src/Resources/Entities/Components/TransformComponent.h"
#include "../src/Resources/Entities/Components/ProjectileComponent.h"

Engine::Engine() :
	_exit			 ( false )
{
	Clock* clock = new Clock;
	_environment.set_clock(clock);

	Window* window = new Window;
	_environment.set_window(window);

	ResourceManager* resource_manager = new ResourceManager;
	_environment.set_resource_manager(resource_manager);
	resource_manager->load_resources();
}

Engine::~Engine() {
	_environment.shut_down();
}

void Engine::run() {

	/*
	_entity = Environment::get().get_resources()->new_entity(ENTITY_PLAYER, 0);
	auto floor = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 1);
	floor->get<TransformComponent>()->set(glm::vec3(0, 5, 0));
	auto box = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 2);
	box->get<TransformComponent>()->set(glm::vec3(5, -.75, 2));
	auto tower = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 3);
	tower->get<TransformComponent>()->set(glm::vec3(-5, -.75, 2));
	auto bridge = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 4);
	bridge->get<TransformComponent>()->set(glm::vec3(10 , -.75, 10));
	*/

	auto level = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 5);

	while (!_exit) {
		_environment.get_clock()->update();
		_environment.get_window()->update();

		render();

		input();

		_environment.get_resources()->update_entities();
	}
	
}

void Engine::input() {
	glfwPollEvents();
	GLFWwindow* window = _environment.get_window()->get_glfw_window();

	if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		return;
	}

	_exit = glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	_environment.get_window()->get_camera()->move_angle((float)xpos, (float)ypos);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);

	if (glfwGetKey(window, GLFW_KEY_W)) {
		_environment.get_window()->get_camera()->move(CAMERA_FORWARD);
	}

	if (glfwGetKey(window, GLFW_KEY_S)) {
		_environment.get_window()->get_camera()->move(CAMERA_BACKWARD);
	}

	if (glfwGetKey(window, GLFW_KEY_A)) {
		_environment.get_window()->get_camera()->move(CAMERA_LEFT);
	}

	if (glfwGetKey(window, GLFW_KEY_D)) {
		_environment.get_window()->get_camera()->move(CAMERA_RIGHT);
	}

	if (glfwGetKey(window, GLFW_KEY_E)) {
		_environment.get_window()->get_camera()->move(CAMERA_UP);
	}

	if (glfwGetKey(window, GLFW_KEY_Q)) {
		_environment.get_window()->get_camera()->move(CAMERA_DOWN);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		_environment.get().get_resources()->get_player()->get<PlayerComponent>()->fire();
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		_environment.get().get_resources()->get_player()->get<PlayerComponent>()->jump();
	}
	
}

void Engine::render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, WINDOW_BACKGROUND_COLOR);

	//Environment::get().get_resources()->get_model(0)->draw();

	//transform->_model->draw(transform->_transform); 
	//Environment::get().get_resources()->get_model(_entity->get_model_id())->draw(transform->_transform);

	_environment.get().get_resources()->render_entities();

	//Environment::get().get_resource_manager()->get_model(1)->draw();

	glfwSwapBuffers(_environment.get_window()->get_glfw_window());
}