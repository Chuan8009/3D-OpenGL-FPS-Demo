#include "Entity.h"

#include "Components/ComponentLoader.h"

unsigned int make_unique_id() {
	static unsigned int id = 0;
	return id++;
}

Entity::Entity() :
	_unique_id  ( make_unique_id() ),
	_id			( -1 ),
	_model_id	( 0 ),
	_destroy	( false ),
	_draw		( false )
{}

Entity::Entity(const std::string_view type, const int id) :
	_unique_id  ( make_unique_id() ),
	_type		( type ),
	_id			( id ),
	_model_id	( 0 ),
	_destroy    ( false ),
	_draw		( true )
{
	// ready to load
}

Entity::Entity(const Entity& rhs) :
	_unique_id	( make_unique_id() ),
	_type		( rhs._type ),
	_id			( rhs._id ),
	_model_id	( rhs._model_id ),
	_name		( rhs._name ),
	_destroy	( rhs._destroy ),
	_draw		( rhs._draw )
{
	// ready to copy
}

Entity::~Entity() 
{}

void Entity::add(std::shared_ptr<Component> component) {
	_components[component->get_type()] = component;
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

void Entity::copy(const Entity& rhs) {
	for (size_t i = 0; i < TOTAL_COMPONENTS; ++i) {
		if (rhs._components[i]) {
			_components[i] = rhs._components[i]->copy(shared_from_this());
		}
	}
}

void Entity::destroy() {
	_destroy = true;
}

unsigned int Entity::get_unique_id() {
	return _unique_id;
}

int Entity::get_id() {
	return _id;
}

std::string Entity::get_type() {
	return _type;
}

int Entity::get_model_id() {
	return _model_id;
}

std::string_view Entity::get_name() {
	return _name;
}

void Entity::set_model_id(const int model_id) {
	_model_id = model_id;
}

void Entity::set_name(const std::string_view name) {
	_name = name;
}

void Entity::set_draw(bool draw) {
	_draw = draw;
}

bool Entity::get_destroy() {
	return _destroy;
}

bool Entity::get_draw() {
	return _draw;
}