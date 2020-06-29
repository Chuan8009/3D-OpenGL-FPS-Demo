#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include "../src/Resources/Model.h"

#define TURN_CLOCKWISE 0
#define TURN_CCLOCKWISE 1

class Entity;

class TransformComponent : public Component {
public:
	TransformComponent(std::shared_ptr<Entity> entity, glm::vec3 scale, glm::vec3 rotation, float speed, bool has_collision);
	TransformComponent(std::shared_ptr<Entity> new_entity, const TransformComponent& rhs);
	std::shared_ptr<Component> copy(std::shared_ptr<Entity> new_entity) const;

	void update();

	void on_collision(std::shared_ptr<Entity> entity);

	const int get_type() const;

	static constexpr int _type = TRANSFORM_COMPONENT;

	void move(glm::vec3 dir);
	void set(const glm::vec3 pos);
	void set_direction(const glm::vec3 dir);

	void update_grid(const glm::vec3 old_position);

	std::shared_ptr<Entity> is_collision();

	Transform _transform;

	glm::vec3 _direction;
	float _y_rot;
	int _turn;

	float _speed;

	bool _has_collision;

};

#endif