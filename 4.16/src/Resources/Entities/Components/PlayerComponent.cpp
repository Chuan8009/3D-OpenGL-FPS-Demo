#include "PlayerComponent.h"

#include "../src/Resources/Entities/Entity.h"

#include "../src/System/Environment.h"
#include "../src/Resources/ResourceManager.h"
#include "../src/Resources/Window.h"
#include "../src/Resources/Camera.h"
#include "../src/Utility/Clock.h"

#include "TransformComponent.h"
#include "ProjectileComponent.h"


PlayerComponent::PlayerComponent(std::shared_ptr<Entity> entity, float fire_rate, float weight) :
	Component			( entity ),
	_fire_rate			( fire_rate ),
	_fire_rate_timer	( fire_rate ),
	_weight				( weight ),
	_jumping			( false ),
	_y_vel				( 0.0f )
{}

PlayerComponent::PlayerComponent(std::shared_ptr<Entity> new_entity, const PlayerComponent& rhs) :
	Component			( new_entity ),
	_fire_rate			( rhs._fire_rate ),
	_fire_rate_timer	( rhs._fire_rate ),
	_weight				( rhs._weight ),
	_jumping			( rhs._jumping ),
	_y_vel				( rhs._y_vel )
{}

std::shared_ptr<Component> PlayerComponent::copy(std::shared_ptr<Entity> new_entity) const {
	return std::static_pointer_cast<Component>(std::make_shared<PlayerComponent>(new_entity, *this));
}

void PlayerComponent::update() {
	auto new_position = Environment::get().get_window()->get_camera()->get_position();
	auto old_position = new_position;
	auto time = Environment::get().get_clock()->get_time();
	new_position.y += _y_vel * (float)time;
	_y_vel -= _weight * (float)time;

	if (_y_vel < -40000) {
		_y_vel = -40000;
	}

	Environment::get().get_window()->get_camera()->set_position(new_position);

	if (Environment::get().get_window()->get_camera()->is_collision()) {
		if (_y_vel > 0) {
			_y_vel = 0;
		}
		else if (_y_vel < 0) {
			_y_vel = 0;
			_jumping = false;
		}
		Environment::get().get_window()->get_camera()->set_position(old_position);
	}
}

const int PlayerComponent::get_type() const {
	return PLAYER_COMPONENT;
}

void PlayerComponent::fire() {
	if (_fire_rate_timer.update()) {
		auto bullet = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 0);
		auto camera_direction = Environment::get().get_window()->get_camera()->get_direction();
		auto camera_position = Environment::get().get_window()->get_camera()->get_position();
		bullet->get<ProjectileComponent>()->fire(camera_direction);
		bullet->get<TransformComponent>()->set(camera_position);
	}
}

void PlayerComponent::jump() {
	if (!_jumping) {
		_jumping = true;
		_y_vel = 25000;
	}
}