#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include "../src/Resources/Model.h"

class Entity;

class TransformComponent : public Component {
public:
	TransformComponent(std::shared_ptr<Entity> entity, glm::vec3 scale, glm::vec3 rotation, float speed, bool has_collision);
	TransformComponent(std::shared_ptr<Entity> new_entity, const TransformComponent& rhs);
	std::shared_ptr<Component> copy(std::shared_ptr<Entity> new_entity) const;

	void update();

	const int get_type() const;

	static constexpr int _type = TRANSFORM_COMPONENT;

	void move(glm::vec3 dir);
	void set(const glm::vec3 pos);

	void update_grid(const glm::vec3 old_position);

	bool is_collision();

	Transform _transform;

	float _speed;

	bool _has_collision;

};

#endif