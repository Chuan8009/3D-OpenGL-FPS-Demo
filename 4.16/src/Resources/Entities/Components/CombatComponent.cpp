#include "CombatComponent.h"

#include "../src/Resources/Entities/Entity.h"

#include "ProjectileComponent.h"

CombatComponent::CombatComponent(std::shared_ptr<Entity> entity, int health, int damage) :
	Component   ( entity ),
	_health		( health ),
	_damage     ( damage )
{}

CombatComponent::CombatComponent(std::shared_ptr<Entity> new_entity, const CombatComponent& rhs) :
	Component   ( new_entity ),
	_health		( rhs._health ),
	_damage		( rhs._damage )
{}

std::shared_ptr<Component> CombatComponent::copy(std::shared_ptr<Entity> new_entity) const {
	return std::static_pointer_cast<Component>(std::make_shared<CombatComponent>(new_entity, *this));
}

void CombatComponent::update() 
{}

void CombatComponent::on_collision(std::shared_ptr<Entity> entity) {
}

const int CombatComponent::get_type() const {
	return COMBAT_COMPONENT;
}

void CombatComponent::hit(std::shared_ptr<CombatComponent> attacker) {
	if (attacker != _entity->get<CombatComponent>()) {
		_health -= attacker->_damage;

		if (_health <= 0) {
			if (_entity->get_type() != ENTITY_PLAYER) {
				_entity->destroy();
			}
		}
	}
}