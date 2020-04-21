#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <string_view>
#include <array>
#include <memory>

#include "Components/Component.h"

class Entity : public std::enable_shared_from_this<Entity> {
public:
	Entity();
	Entity(const std::string_view type, const int id);
	Entity(const Entity& rhs);
	~Entity();

	void add(std::shared_ptr<Component> component);

	template<typename T>
	std::shared_ptr<T> get();

	void update();

	void clear();

	int get_id();
	std::string get_type();

	void set_name(const std::string_view name);

private:
	int _id;
	std::string _type;
	std::string _name;

	std::array<std::shared_ptr<Component>, TOTAL_COMPONENTS> _components{ nullptr };
};

#endif