#include "Window.h"

#include "FileReader.h"

#include "Environment.h"
#include "Clock.h"

#include "Camera.h"

#include "Timer.h"

#define FILE_WINDOW_WIDTH "i_width"
#define FILE_WINDOW_HEIGHT "i_height"
#define FILE_WINDOW_TITLE "str_title"
#define FILE_WINDOW_X_POS "i_x_pos"
#define FILE_WINDOW_Y_POS "i_y_pos"

Window_Settings load_window_settings(const char* file_path) {
	FileReader file(file_path);
	Window_Settings settings;

	file.set_section("Window");
	file.read(&settings.width, FILE_WINDOW_WIDTH);
	file.read(&settings.height, FILE_WINDOW_HEIGHT);
	file.read(&settings.title, FILE_WINDOW_TITLE);
	file.read(&settings.x_pos, FILE_WINDOW_X_POS);
	file.read(&settings.y_pos, FILE_WINDOW_Y_POS);

	return settings;
}

Window::Window(const Window_Settings settings) :
	_window ( glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr) )
{
	glfwMakeContextCurrent(_window);
	glfwSetWindowPos(_window, settings.x_pos, settings.y_pos);

	gl3wInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	_camera = new Camera(settings.width, settings.height, 90.0f, 1.0f, 0.1f, 1000.0f, 3.14f, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
}

Window::~Window() {
	delete _camera;
}

void Window::update() {
	static Timer update_title_timer(1.0);

	if (update_title_timer.update()) {
		glfwSetWindowTitle(_window, std::to_string(Environment::get().get_clock()->get_fms()).c_str());
	}

	_camera->update();
}

GLFWwindow* Window::get_glfw_window() {
	return _window;
}

Camera* Window::get_camera() {
	return _camera;
}