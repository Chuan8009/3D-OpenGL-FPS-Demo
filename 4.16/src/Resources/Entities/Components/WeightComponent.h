#ifndef WEIGHT_COMPONENT_H
#define WEIGHT_COMPONENT_H

#include "Component.h"

class Entity;

class WeightComponent : public Component {
public:
	WeightComponent(std::shared_ptr<Entity> entity, float weight);
	WeightComponent(std::shared_ptr<Entity> new_entity, const WeightComponent& rhs);
	std::shared_ptr<Component> copy(std::shared_ptr<Entity> new_entity) const;

	void update();

	void on_collision(std::shared_ptr<Entity> entity);

	const int get_type() const;

	static constexpr int _type = WEIGHT_COMPONENT;

	void jump();

	bool _jumping;
	float _weight;
	float _y_vel;
};

#endif