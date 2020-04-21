#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <memory>

#include "Component.h"

class Entity;

class TransformComponent : public Component {
public:
	TransformComponent(std::shared_ptr<Entity> entity);
	TransformComponent(std::shared_ptr<Entity> new_entity, const TransformComponent& rhs);
	std::shared_ptr<Component> copy(std::shared_ptr<Entity> new_entity) const;

	void update();

	const int get_type() const;

	static constexpr int _type = COMPONENT_TRANSFORM;
};

#endif