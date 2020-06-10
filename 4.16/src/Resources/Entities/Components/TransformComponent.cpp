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
	auto entities = Environment::get().get_resources()->get_entities();

	for (auto e : *entities) {
		if (e->get_unique_id() != _entity->get_unique_id()) {
			if (auto e_transform = e->get<TransformComponent>()) {
				auto a_model = Environment::get().get_resources()->get_model(_entity->get_model_id());
				auto b_model = Environment::get().get_resources()->get_model(e->get_model_id());

				for (auto a_mesh : a_model->_meshes) {
					for (auto b_mesh : b_model->_meshes) {

						Bounding_Box a = a_mesh._bounding_box;
						a.min.x *= _transform.get_scale().x;
						a.min.y *= _transform.get_scale().y;
						a.min.z *= _transform.get_scale().z;
						a.max.x *= _transform.get_scale().x;
						a.max.y *= _transform.get_scale().y;
						a.max.z *= _transform.get_scale().z;
						a.min += _transform.get_position();
						a.max += _transform.get_position();

						Bounding_Box b = b_mesh._bounding_box;
						b.min.x *= e_transform->_transform.get_scale().x;
						b.min.y *= e_transform->_transform.get_scale().y;
						b.min.z *= e_transform->_transform.get_scale().z;
						b.max.x *= e_transform->_transform.get_scale().x;
						b.max.y *= e_transform->_transform.get_scale().y;
						b.max.z *= e_transform->_transform.get_scale().z;
						b.min += e_transform->_transform.get_position();
						b.max += e_transform->_transform.get_position();

						if (collision(a, b)) {
							return true;
						}
					}
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
}

void TransformComponent::set(const glm::vec3 pos) {
	_transform.set_position(pos);
}