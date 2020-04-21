#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <array>
#include <map>

#include <memory>

struct Program;
class Model;
class Entity;

typedef unsigned int GLuint;

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	GLuint get_program(const size_t id);
	std::shared_ptr<Model> get_model(const size_t id);

	void load_resources();
private:
	std::map<size_t, GLuint> _programs;
	std::map<size_t, std::shared_ptr<Model>> _models;
	std::map<std::string_view, std::map<size_t, std::shared_ptr<Entity>>> _entities;

	bool _load_programs();
	bool _load_program(const size_t id, const std::string_view path);

	bool _load_models();
	bool _load_model(const size_t model, const std::string_view path);

	bool _load_entities();
};

#endif