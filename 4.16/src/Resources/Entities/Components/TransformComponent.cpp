#include "TransformComponent.h"

#include "../src/System/Environment.h"
#include "../src/Resources/ResourceManager.h"

#include "../src/Resources/Entities/Entity.h"
#include "../src/Utility/Collision.h"

TransformComponent::TransformComponent(std::shared_ptr<Entity> entity, glm::vec3 scale, glm::vec3 rotation, float speed) :
	Component		( entity ),
	_transform		( glm::vec3(0.0, 0.0, 0.0), scale, rotation ),
	_speed			( speed )
{}

TransformComponent::TransformComponent(std::shared_ptr<Entity> new_entity, const TransformComponent& rhs) :
	Component		( new_entity ),
	_transform		( rhs._transform ),
	_speed			( rhs._speed )
{}

std::shared_ptr<Component> TransformComponent::copy(std::shared_ptr<Entity> new_entity) const {
	return std::static_pointer_cast<Component>(std::make_shared<TransformComponent>(new_entity, *this));
}

void TransformComponent::update() {
}

bool TransformComponent::is_collision() {
	for(auto& mesh : Environment::get().get_resources()->get_model(_entity->get_model_id())->_meshes) {
		Bounding_Box a = mesh._bounding_box;
		a.min *= _transform.get_scale();
		a.max *= _transform.get_scale();
		a.min += _transform.get_position();
		a.max += _transform.get_position();

		auto cells = Environment::get().get_resources()->get_entity_grid()->get_cells(a);
		for (auto vec : cells) {
			for (auto e : *vec) {
				if (e.second != _entity && collision(a, e.first)) {
					return true;
				}
			}
		}

	}

	return false;
}

const int TransformComponent::get_type() const {
	return _type;
}

void TransformComponent::move(glm::vec3 dir) {
	dir = glm::normalize(dir);

	glm::vec3 old_position = _transform.get_position();

	_transform.set_position(
		glm::vec3(
		_transform.get_position().x + dir.x * _speed,
		_transform.get_position().y + dir.y * _speed,
		_transform.get_position().z + dir.z * _speed
		)
	);

	if(is_collision()) {
		_transform.set_position(old_position);
	}
	else {
		update_grid(old_position);
	}
}

void TransformComponent::set(const glm::vec3 pos) {
	_transform.set_position(pos);
}

void TransformComponent::update_grid(const glm::vec3 old_position) {
	for (auto& mesh : Environment::get().get_resources()->get_model(_entity->get_model_id())->_meshes) {
		Bounding_Box a = mesh._bounding_box;
		a.min *= _transform.get_scale();
		a.max *= _transform.get_scale();
		a.min += old_position;
		a.max += old_position;
		Environment::get().get_resources()->get_entity_grid()->remove(a, _entity);

		a = mesh._bounding_box;
		a.min *= _transform.get_scale();
		a.max *= _transform.get_scale();
		a.min += _transform.get_position();
		a.max += _transform.get_position();

		Environment::get().get_resources()->get_entity_grid()->insert(a, _entity);
	}
}