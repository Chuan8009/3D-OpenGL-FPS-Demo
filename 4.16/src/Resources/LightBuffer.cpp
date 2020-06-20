#include "LightBuffer.h"

#include "../src/Resources/Entities/Entity.h"
#include "../src/Resources/Entities/Components/TransformComponent.h"

void LightBuffer::add(std::shared_ptr<LightComponent> new_light) {
	for(auto& light : _lights) {
		if(light == nullptr) {
			light = new_light;

			int index = _num_lights * 3;
			_light_colors[index] = light->_color.x;
			_light_colors[index + 1] = light->_color.y;
			_light_colors[index + 2] = light->_color.z;

			auto position = light->_entity->get<TransformComponent>()->_transform.get_position();
			_light_positions[index] = position.x + light->_offset.x;
			_light_positions[index + 1] = position.y + light->_offset.y;
			_light_positions[index + 2] = position.z + light->_offset.z;

			_light_intensities[_num_lights] = light->_intensity;
			++_num_lights;
			break;
		}
	}
}

void LightBuffer::remove(std::shared_ptr<LightComponent> old_light) {

}

void LightBuffer::set_static_point() {
	_static_point = _num_lights;
}

void LightBuffer::clear() {
	_num_lights = _static_point;

	for(int i = _num_lights; i < MAX_LIGHTS; ++i) {
		_lights[i] = nullptr;
	}
}

void LightBuffer::update() {
	glUseProgram(_program);
	glUniform1i(glGetUniformLocation(_program, "num_lights"), _num_lights);
	glUniform3fv(glGetUniformLocation(_program, "light_color"), _num_lights, &_light_colors[0]);
	glUniform3fv(glGetUniformLocation(_program, "light_position"), _num_lights, &_light_positions[0]);
	glUniform1fv(glGetUniformLocation(_program, "light_intensity"), _num_lights, &_light_intensities[0]);
}