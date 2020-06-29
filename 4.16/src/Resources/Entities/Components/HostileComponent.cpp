#include "HostileComponent.h"

#include "../src/Resources/Entities/Entity.h"
#include "TransformComponent.h"

#include "../src/System/Environment.h"
#include "../src/Resources/ResourceManager.h"
#include "../src/Resources/Window.h"
#include "../src/Resources/Camera.h"

#include "ProjectileComponent.h"

HostileComponent::HostileComponent(std::shared_ptr<Entity> entity, float fire_rate) :
	Component		 ( entity ),
	_fire_rate		 ( fire_rate ),
	_fire_rate_timer ( fire_rate )
{}

HostileComponent::HostileComponent(std::shared_ptr<Entity> new_entity, const HostileComponent& rhs) :
	Component		 ( new_entity ),
	_fire_rate		 ( rhs._fire_rate ),
	_fire_rate_timer ( rhs._fire_rate )
{}

std::shared_ptr<Component> HostileComponent::copy(std::shared_ptr<Entity> new_entity) const {
	return std::static_pointer_cast<Component>(std::make_shared<HostileComponent>(new_entity, *this));
}

void HostileComponent::update() {
	update_direction();

	fire();
}

void HostileComponent::update_direction() {
	auto camera_position = Environment::get().get_window()->get_camera()->get_position();
	auto position = _entity->get<TransformComponent>()->_transform.get_position();
	camera_position -= position;
	auto direction = glm::normalize(camera_position);
	if (direction.y < 0) direction.y = 0;
	_entity->get<TransformComponent>()->move(direction);
	_entity->get<TransformComponent>()->set_direction(direction);
}

void HostileComponent::fire() {
	if (_fire_rate_timer.update()) {
		auto bullet = Environment::get().get_resources()->new_entity(ENTITY_OBJECT, 0, true);
		auto direction = _entity->get<TransformComponent>()->_direction;
		auto position = _entity->get<TransformComponent>()->_transform.get_position();
		bullet->get<ProjectileComponent>()->fire(direction);
		position.z += 5.0f * direction.z;
		position.x += 5.0f * direction.x;
		position.y -= 1.0f;
		bullet->get<TransformComponent>()->set(position);
		bullet->get<ProjectileComponent>()->_owner = _entity;
	}
}

void HostileComponent::on_collision(std::shared_ptr<Entity> entity) {

}

const int HostileComponent::get_type() const {
	return HOSTILE_COMPONENT;
}