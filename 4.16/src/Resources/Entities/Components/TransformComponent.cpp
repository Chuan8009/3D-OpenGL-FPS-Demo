#include "TransformComponent.h"

TransformComponent::TransformComponent(std::shared_ptr<Entity> entity) :
	Component ( entity )
{}

TransformComponent::TransformComponent(std::shared_ptr<Entity> new_entity, const TransformComponent& rhs) :
	Component ( new_entity )
{}

std::shared_ptr<Component> TransformComponent::copy(std::shared_ptr<Entity> new_entity) const {
	return std::static_pointer_cast<Component>(std::make_shared<TransformComponent>(new_entity, *this));
}

void TransformComponent::update() {

}

const int TransformComponent::get_type() const {
	return _type;
}