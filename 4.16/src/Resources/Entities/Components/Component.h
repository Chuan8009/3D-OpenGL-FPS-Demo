#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

enum
{
	PLAYER_COMPONENT,
	TRANSFORM_COMPONENT,
	PROJECTILE_COMPONENT,
	LIGHT_COMPONENT,
	HOSTILE_COMPONENT,
	WEIGHT_COMPONENT,
	COMBAT_COMPONENT,

	TOTAL_COMPONENTS
};

class Entity;

class Component {
public:
	Component(std::shared_ptr<Entity> entity) :
		_entity		(entity)
	{}
	~Component() {}

	virtual std::shared_ptr<Component> copy(std::shared_ptr<Entity> new_entity) const = 0;

	virtual void update() = 0;

	virtual void on_collision(std::shared_ptr<Entity> entity) = 0;

	virtual const int get_type() const = 0;

	std::shared_ptr<Entity> _entity;
};

#endif