#include "Window.h"

#include "FileReader.h"

#include "Environment.h"
#include "Clock.h"

#include "Timer.h"

#define FILE_WINDOW_WIDTH "i_width"
#define FILE_WINDOW_HEIGHT "i_height"
#define FILE_WINDOW_TITLE "str_title"
#define FILE_WINDOW_X_POS "i_x_pos"
#define FILE_WINDOW_Y_POS "i_y_pos"

Window::Window(const Settings settings) :
	_window ( glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr) )
{
	glfwMakeContextCurrent(_window);
	glfwSetWindowPos(_window, settings.x_pos, settings.y_pos);

	gl3wInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Window::update() {
	static Timer update_title_timer(1.0);

	if (update_title_timer.update()) {
		glfwSetWindowTitle(_window, std::to_string(Environment::get().get_clock()->get_fms()).c_str());
	}
}

GLFWwindow* Window::get_glfw_window() {
	return _window;
}

const Window::Settings Window::load_settings(const char* file_path) {
	FileReader file(file_path);
	Window::Settings settings;

	file.set_section("Window");
	file.read_int(&settings.width, FILE_WINDOW_WIDTH);
	file.read_int(&settings.height, FILE_WINDOW_HEIGHT);
	file.read_string(&settings.title, FILE_WINDOW_TITLE);
	file.read_int(&settings.x_pos, FILE_WINDOW_X_POS);
	file.read_int(&settings.y_pos, FILE_WINDOW_Y_POS);

	return settings;
}