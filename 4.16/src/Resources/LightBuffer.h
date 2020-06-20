#ifndef LIGHT_BUFFER_H
#define LIGHT_BUFFER_H

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <array>

#include "../src/Resources/Entities/Components/LightComponent.h"

constexpr int MAX_LIGHTS = 25;

struct LightBuffer {
	void add(std::shared_ptr<LightComponent> light);
	void remove(std::shared_ptr<LightComponent> light);
	void set_static_point();
	void clear();
	void update();

	std::array<std::shared_ptr<LightComponent>, MAX_LIGHTS> _lights;
	std::array<GLfloat, MAX_LIGHTS * 3> _light_colors;
	std::array<GLfloat, MAX_LIGHTS * 3> _light_positions;
	std::array<GLfloat, MAX_LIGHTS> _light_intensities;
	int _num_lights = 0;
	int _static_point = 0;

	GLuint _program;
};

#endif