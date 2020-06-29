#include "TransformComponent.h"

#include "../src/System/Environment.h"
#include "../src/Resources/ResourceManager.h"

#include "../src/Resources/Entities/Entity.h"
#include "../src/Resources/Entities/Components/ProjectileComponent.h"
#include "../src/Utility/Collision.h"

#include "../src/Utility/Clock.h"

constexpr float ROTATION_SPEED = 100000.0f;

TransformComponent::TransformComponent(std::shared_ptr<Entity> entity, glm::vec3 scale, glm::vec3 rotation, float speed, bool has_collision) :
	Component		( entity ),
	_transform		( glm::vec3(0.0, 0.0, 0.0), scale, rotation ),
	_speed			( speed ),
	_has_collision	( has_collision ),
	_y_rot			( 0 ),
	_turn			( 0 )
{}

TransformComponent::TransformComponent(std::shared_ptr<Entity> new_entity, const TransformComponent& rhs) :
	Component		( new_entity ),
	_transform		( rhs._transform ),
	_speed			( rhs._speed ),
	_has_collision	( rhs._has_collision ),
	_y_rot			( rhs._y_rot ),
	_turn			( rhs._turn )
{}

std::shared_ptr<Component> TransformComponent::copy(std::shared_ptr<Entity> new_entity) const {
	return std::static_pointer_cast<Component>(std::make_shared<TransformComponent>(new_entity, *this));
}

void TransformComponent::update() {
	auto rotation = _transform.get_rotation();
	if (_y_rot != rotation.y) {
		if(_turn == TURN_CLOCKWISE) {
			rotation.y -= ROTATION_SPEED * (float)Environment::get().get_clock()->get_time();
			if(rotation.y < 0.0f) {
				rotation.y += 360.0f;
			}
			if (rotation.y < _y_rot) {
				rotation.y = _y_rot;
			}
		}
		else if(_turn == TURN_CCLOCKWISE) {
			rotation.y += ROTATION_SPEED * (float)Environment::get().get_clock()->get_time();
			if(rotation.y > 360.0f) {
				rotation.y -= 360.0f;
			}
			if(rotation.y > _y_rot) {
				rotation.y = _y_rot;
			}
		}
		_transform.set_rotation(rotation);
	}
}

void TransformComponent::on_collision(std::shared_ptr<Entity> entity) {

}

std::shared_ptr<Entity> TransformComponent::is_collision() {
	if(!_has_collision) {
		return nullptr;
	}

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
					return e.second;
				}
			}
		}
	}

	return nullptr;
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

	if(_entity->is_collision()) {
		_transform.set_position(old_position);
	}
	else {
		_direction = dir;
		update_grid(old_position);
	}
}

void TransformComponent::set(const glm::vec3 pos) {
	_transform.set_position(pos);
}

void TransformComponent::set_direction(const glm::vec3 dir) {
	float old_y_rot = _y_rot;
	_direction = dir;

	if (dir.z < 0 && dir.x < 0) {
		_y_rot = 180.0f - dir.x * 90.0f;
	}
	else if (dir.z < 0 && dir.x > 0) {
		_y_rot = 180.0f - dir.x * 90.0f;
	}
	else if (dir.z > 0 && dir.x < 0) {
		_y_rot = 360.0f + dir.x * 90.0f;
	}
	else {
		_y_rot = dir.x * 90.0f;
	}

	float dif = old_y_rot - _y_rot;
	if(dif < 0.0f) {
		_turn = TURN_CCLOCKWISE;
	}
	else {
		_turn = TURN_CLOCKWISE;
	}
}

void TransformComponent::update_grid(const glm::vec3 old_position) {
	if(!_has_collision) {
		return;
	}

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