#include "WeightComponent.h"

#include "../src/Resources/Entities/Entity.h"
#include "../src/Resources/Entities/Components/TransformComponent.h"

#include "../src/System/Environment.h"
#include "../src/Utility/Clock.h"
#include "../src/Resources/Window.h"
#include "../src/Resources/Camera.h"

constexpr int MAX_Y_VEL = 40000;
constexpr int JUMP_VEL = 25000;

WeightComponent::WeightComponent(std::shared_ptr<Entity> entity, float weight) :
	Component	( entity  ),
	_weight		( weight ),
	_jumping	( false ),
	_y_vel		( 0 )
{}

WeightComponent::WeightComponent(std::shared_ptr<Entity> new_entity, const WeightComponent& rhs) :
	Component	( new_entity ),
	_weight		( rhs._weight),
	_jumping	( rhs._jumping ),
	_y_vel		( rhs._y_vel )
{}

std::shared_ptr<Component> WeightComponent::copy(std::shared_ptr<Entity> new_entity) const {
	return std::static_pointer_cast<Component>(std::make_shared<WeightComponent>(new_entity, *this));
}

void WeightComponent::update() {
	glm::vec3 new_position;
	if (_entity->get_type() == ENTITY_PLAYER) {
		new_position = Environment::get().get_window()->get_camera()->get_position();
	}
	else {
		new_position = _entity->get<TransformComponent>()->_transform.get_position();
	}
	auto old_position = new_position;
	auto time = Environment::get().get_clock()->get_time();
	new_position.y += _y_vel * (float)time;
	_y_vel -= _weight * (float)time;

	if (_y_vel < -MAX_Y_VEL) {
		_y_vel = -MAX_Y_VEL;
	}

	if (_entity->get_type() == ENTITY_PLAYER) {
		Environment::get().get_window()->get_camera()->set_position(new_position);
	}
	else {
		_entity->get<TransformComponent>()->_transform.set_position(new_position);
		_entity->get<TransformComponent>()->update_grid(old_position);
	}

	if (_entity->is_collision()) {
		if (_y_vel > 0) {
			_y_vel = 0;
		}
		else if (_y_vel < 0) {
			_y_vel = 0;
			_jumping = false;
		}

		if (_entity->get_type() == ENTITY_PLAYER) {
			Environment::get().get_window()->get_camera()->set_position(old_position);
		}
		else {
			_entity->get<TransformComponent>()->_transform.set_position(old_position);
		}
	}
}

void WeightComponent::on_collision(std::shared_ptr<Entity> entity) {
}

void WeightComponent::jump() {
	if (!_jumping) {
		_jumping = true;
		_y_vel = JUMP_VEL;
	}
}

const int WeightComponent::get_type() const {
	return WEIGHT_COMPONENT;
}