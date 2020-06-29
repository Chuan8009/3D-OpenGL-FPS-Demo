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
#include "../src/Resources/Entities/Components/WeightComponent.h"

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
	level->get<TransformComponent>()->set(glm::vec3(100, 0, 100));

	auto enemy = Environment::get().get_resources()->new_entity(ENTITY_ENEMY, 0);
	enemy->get<TransformComponent>()->set(glm::vec3(100, 200, 100));

	make_lights();

	Environment::get().get_resources()->build_entity_grid();


	while (!_exit) {
		_environment.get_clock()->update();
		_environment.get_window()->update();

		render();

		input();

		spawn_enemies();
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
		_environment.get().get_resources()->get_player()->get<WeightComponent>()->jump();
	}
	
}

void Engine::render() {
	//static PointLight light(Environment::get().get_resources()->get_program(4), Transform(glm::vec3(100, 0, 100)));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, WINDOW_BACKGROUND_COLOR);

	//Environment::get().get_resources()->get_model(0)->draw();

	//transform->_model->draw(transform->_transform); 
	//Environment::get().get_resources()->get_model(_entity->get_model_id())->draw(transform->_transform);

	//light.test();
	_environment.get().get_resources()->render_entities();

	//debug_draw();

	//Environment::get().get_resource_manager()->get_model(1)->draw();

	glfwSwapBuffers(_environment.get_window()->get_glfw_window());
}

void Engine::debug_draw() {
	auto grid = Environment::get().get_resources()->get_entity_grid();
	static std::vector<glm::vec3> points;
	static bool draw = false;
	static Mesh lines;
	static Transform transform;

	if (!draw) {
		for (int i = 0; i < grid->_width; i += grid->_cell_width) {
			points.push_back(glm::vec3(i, .1, -(grid->_height / 2)));
			points.push_back(glm::vec3(i, .1, (grid->_height / 2)));
		}

		for (int i = 0; i < grid->_height; i += grid->_cell_height) {
			points.push_back(glm::vec3(-(grid->_width / 2), .1, i));
			points.push_back(glm::vec3((grid->_width / 2), .1, i));
		}

		lines._vertices = points;
		lines.load_buffers();

		draw = true;
	}

	lines.draw_lines(Environment::get().get_resources()->get_program(2), transform);
}

void Engine::make_lights() {
	auto light = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 6);
	light->get<TransformComponent>()->set(glm::vec3(110, 5, 105));
	Environment::get().get_resources()->get_light_buffer().add(light->get<LightComponent>());

	auto light2 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 6);
	light2->get<TransformComponent>()->set(glm::vec3(90, 5, 90));
	Environment::get().get_resources()->get_light_buffer().add(light2->get<LightComponent>());

	auto light3 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 6);
	light3->get<TransformComponent>()->set(glm::vec3(50, 6, 72));
	Environment::get().get_resources()->get_light_buffer().add(light3->get<LightComponent>());

	auto light4 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 8);
	light4->get<TransformComponent>()->set(glm::vec3(65, 5, 48));
	Environment::get().get_resources()->get_light_buffer().add(light4->get<LightComponent>());

	auto light5 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 8);
	light5->get<TransformComponent>()->set(glm::vec3(65, 6, 115));
	Environment::get().get_resources()->get_light_buffer().add(light5->get<LightComponent>());

	auto light6 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 6);
	light6->get<TransformComponent>()->set(glm::vec3(115, 5, 48));
	Environment::get().get_resources()->get_light_buffer().add(light6->get<LightComponent>());

	auto light7 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 8);
	light7->get<TransformComponent>()->set(glm::vec3(140, 5, 60));
	Environment::get().get_resources()->get_light_buffer().add(light7->get<LightComponent>());

	auto light8 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 6);
	light8->get<TransformComponent>()->set(glm::vec3(32, 5, 132));
	Environment::get().get_resources()->get_light_buffer().add(light8->get<LightComponent>());

	auto light9 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 6);
	light9->get<TransformComponent>()->set(glm::vec3(121, 5, 134));
	Environment::get().get_resources()->get_light_buffer().add(light9->get<LightComponent>());

	auto light10 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 8);
	light10->get<TransformComponent>()->set(glm::vec3(88, 5, 125));
	Environment::get().get_resources()->get_light_buffer().add(light10->get<LightComponent>());

	auto bulb = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 7);
	bulb->get<TransformComponent>()->set(glm::vec3(70, 14, 100));
	Environment::get().get_resources()->get_light_buffer().add(bulb->get<LightComponent>());

	auto bulb2 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 7);
	bulb2->get<TransformComponent>()->set(glm::vec3(80, 14, 100));
	Environment::get().get_resources()->get_light_buffer().add(bulb2->get<LightComponent>());

	auto bulb3 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 7);
	bulb3->get<TransformComponent>()->set(glm::vec3(90, 14, 100));
	Environment::get().get_resources()->get_light_buffer().add(bulb3->get<LightComponent>());

	auto bulb4 = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 7);
	bulb4->get<TransformComponent>()->set(glm::vec3(140, 40, 100));
	Environment::get().get_resources()->get_light_buffer().add(bulb4->get<LightComponent>());

	Environment::get().get_resources()->get_light_buffer().set_static_point();
}

void Engine::spawn_enemies() {
	static Timer spawn_timer(5);

	if(spawn_timer.update()) {
		auto enemy = _environment.get().get_resources()->new_entity(ENTITY_ENEMY, 0);
		glm::vec3 position(((rand() % 50) + 70), 50, ((rand() % 70) + 50));
		enemy->get<TransformComponent>()->set(position);
	}
}