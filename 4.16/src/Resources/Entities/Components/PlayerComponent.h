#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "Component.h"

#include "../src/Utility/Timer.h"

class Entity;

class PlayerComponent : public Component {
public:
	PlayerComponent(std::shared_ptr<Entity> entity, float fire_rate);
	PlayerComponent(std::shared_ptr<Entity> new_entity, const PlayerComponent& rhs);
	std::shared_ptr<Component> copy(std::shared_ptr<Entity> new_entity) const;

	void update();

	void on_collision(std::shared_ptr<Entity> entity);

	const int get_type() const;

	static constexpr int _type = PLAYER_COMPONENT;

	std::shared_ptr<Entity> is_collision();

	void fire();

	float _fire_rate;
	Timer _fire_rate_timer;
};

#endif