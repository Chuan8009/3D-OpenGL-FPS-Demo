#ifndef WINDOW_H
#define WINODW_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <string>

#define WINDOW_FILE "Data/system.txt"

constexpr GLfloat WINDOW_BACKGROUND_COLOR[4] = { 0.0, 0.0, 0.0, 1.0 };

class Camera;

class Window {
public:
	struct Settings {
		int width = 1000, height = 800;
		std::string title = "Window";
		int x_pos, y_pos;
	};

	Window(const Settings settings = load_settings(WINDOW_FILE));
	~Window();

	void update();

	GLFWwindow* get_glfw_window();
	Camera* get_camera();

	static Settings load_settings(const char* file_path = WINDOW_FILE);
private:
	GLFWwindow* _window;
	Camera* _camera;
};

#endif