#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include "Component.h"

#include <GL/gl3w.h>
#include <glm/gtc/matrix_transform.hpp>

class Entity;

class LightComponent : public Component, public std::enable_shared_from_this<LightComponent> {
public:
	LightComponent(std::shared_ptr<Entity> entity, int program_id, glm::vec3 color, float intensity, bool static_position, glm::vec3 offset);
	LightComponent(std::shared_ptr<Entity> new_entity, const LightComponent& rhs);
	std::shared_ptr<Component> copy(std::shared_ptr<Entity> new_entity) const;

	void update();

	const int get_type() const;

	static constexpr int _type = LIGHT_COMPONENT;

	GLuint _program;
	glm::vec3 _color;
	float _intensity;
	glm::vec3 _offset;
	bool _static_position;
};

#endif