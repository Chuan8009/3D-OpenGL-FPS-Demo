#ifndef COMBAT_COMPONENT_H
#define COMBAT_COMPONENT_H

#include "Component.h"

class Entity;

class CombatComponent : public Component {
public:
	CombatComponent(std::shared_ptr<Entity> entity, int health, int damage);
	CombatComponent(std::shared_ptr<Entity> new_entity, const CombatComponent& rhs);
	std::shared_ptr<Component> copy(std::shared_ptr<Entity> new_entity) const;

	void update();

	void on_collision(std::shared_ptr<Entity> entity);

	const int get_type() const;

	static constexpr int _type = COMBAT_COMPONENT;

	void hit(std::shared_ptr<CombatComponent> attacker);

	int _health;
	int _damage;
};

#endif