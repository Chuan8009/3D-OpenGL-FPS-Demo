#include "Entity.h"

#include "Components/ComponentLoader.h"

Entity::Entity() :
	_id			( -1 )
{}

Entity::Entity(const std::string_view type, const int id) :
	_type		( type ),
	_id			( id )
{}

Entity::Entity(const Entity& rhs) :
	_type		( rhs._type ),
	_id			( rhs._id ),
	_name		( rhs._name )
{
	// copy components
	for (size_t i = 0; i < TOTAL_COMPONENTS; ++i) {
		if (rhs._components[i]) {
			_components[i] = rhs._components[i]->copy(shared_from_this());
		}
	}
}

Entity::~Entity() 
{}

void Entity::add(std::shared_ptr<Component> component) {
	_components[component->get_type()] = component;
}

template<typename T>
std::shared_ptr<T> Entity::get() {
	return static_cast<T>(_components[T::_type]);
}

void Entity::update() {
	for(auto c : _components) {
		if (c) {
			c->update();
		}
	}
}

void Entity::clear() {
	for(auto c : _components) {
		c = nullptr;
	}
}

int Entity::get_id() {
	return _id;
}

std::string Entity::get_type() {
	return _type;
}

void Entity::set_name(const std::string_view name) {
	_name = name;
}