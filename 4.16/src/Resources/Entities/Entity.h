#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <string_view>
#include <array>
#include <memory>

#include "Components/Component.h"

constexpr const char* ENTITY_PLAYER = "Player";
constexpr const char* ENTITY_OBJECT = "Object";
constexpr const char* ENTITY_ENEMY = "Enemy";

class Entity : public std::enable_shared_from_this<Entity> {
public:
	Entity();
	Entity(const std::string_view type, const int id);
	Entity(const Entity& rhs);
	~Entity();

	void add(std::shared_ptr<Component> component);

	template<typename _Component>
	std::shared_ptr<_Component> get() {
		return std::static_pointer_cast<_Component>(_components[_Component::_type]);
	}

	void update();

	void clear();

	void copy(const Entity& rhs);

	void destroy();

	std::shared_ptr<Entity> is_collision();

	unsigned int get_unique_id();
	int get_id();
	std::string get_type();
	int get_model_id();
	std::string_view get_name();
	bool get_destroy();
	bool get_draw();

	void set_model_id(const int model_id);
	void set_name(const std::string_view name);
	void set_draw(bool draw);
private:
	unsigned int _unique_id;
	int _id;
	int _model_id;
	std::string _type;
	std::string _name;

	bool _draw;
	bool _destroy;

	std::array<std::shared_ptr<Component>, TOTAL_COMPONENTS> _components{ nullptr };
};

#endif