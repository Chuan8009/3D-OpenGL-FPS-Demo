#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "Component.h"

#include "../src/Utility/Timer.h"

class Entity;

class PlayerComponent : public Component {
public:
	PlayerComponent(std::shared_ptr<Entity> entity, float fire_rate, float weight);
	PlayerComponent(std::shared_ptr<Entity> new_entity, const PlayerComponent& rhs);
	std::shared_ptr<Component> copy(std::shared_ptr<Entity> new_entity) const;

	void update();

	const int get_type() const;

	static constexpr int _type = PLAYER_COMPONENT;

	void fire();
	void jump();

	float _fire_rate;
	Timer _fire_rate_timer;

	bool _jumping;
	float _weight;
	float _y_vel;
};

#endif