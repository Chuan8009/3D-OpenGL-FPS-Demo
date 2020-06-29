#include "PlayerComponent.h"

#include "../src/Resources/Entities/Entity.h"

#include "../src/System/Environment.h"
#include "../src/Resources/ResourceManager.h"
#include "../src/Resources/Window.h"
#include "../src/Resources/Camera.h"
#include "../src/Utility/Clock.h"

#include "TransformComponent.h"
#include "ProjectileComponent.h"


PlayerComponent::PlayerComponent(std::shared_ptr<Entity> entity, float fire_rate) :
	Component			( entity ),
	_fire_rate			( fire_rate ),
	_fire_rate_timer	( fire_rate )
{}

PlayerComponent::PlayerComponent(std::shared_ptr<Entity> new_entity, const PlayerComponent& rhs) :
	Component			( new_entity ),
	_fire_rate			( rhs._fire_rate ),
	_fire_rate_timer	( rhs._fire_rate )
{}

std::shared_ptr<Component> PlayerComponent::copy(std::shared_ptr<Entity> new_entity) const {
	return std::static_pointer_cast<Component>(std::make_shared<PlayerComponent>(new_entity, *this));
}

void PlayerComponent::update() {

}

void PlayerComponent::on_collision(std::shared_ptr<Entity> entity) {

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
		camera_position.z += .3f * camera_direction.z;
		camera_position.x += .3f * camera_direction.x;
		bullet->get<TransformComponent>()->set(camera_position);
		bullet->get<ProjectileComponent>()->_owner = _entity;
	}
}

std::shared_ptr<Entity> PlayerComponent::is_collision() {
	Bounding_Box a = Environment::get().get_window()->get_camera()->get_bounding_box();
	a.min += Environment::get().get_window()->get_camera()->get_position();
	a.max += Environment::get().get_window()->get_camera()->get_position();

	auto cells = Environment::get().get_resources()->get_entity_grid()->get_cells(a);
	for (auto vec : cells) {
		for (auto e : *vec) {
			if (collision(a, e.first)) {
				return e.second;
			}
		}
	}

	return nullptr;
}