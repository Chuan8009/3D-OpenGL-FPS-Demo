#ifndef HOSTILE_COMPONENT_H
#define HOSTILE_COMPONENT_H

#include "Component.h"

#include "../src/Utility/Timer.h"

class Entity;

class HostileComponent : public Component {
public:
	HostileComponent(std::shared_ptr<Entity> entity, float fire_rate);
	HostileComponent(std::shared_ptr<Entity> new_entity, const HostileComponent& rhs);
	std::shared_ptr<Component> copy(std::shared_ptr<Entity> new_entity) const;

	void update();

	void on_collision(std::shared_ptr<Entity> entity);

	const int get_type() const;

	static constexpr int _type = HOSTILE_COMPONENT;

	void update_direction();
	void fire();

	float _fire_rate;
	Timer _fire_rate_timer;
};

#endif