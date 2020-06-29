#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <array>
#include <vector>
#include <map>

#include <memory>

#include "../src/Utility/Grid.h"
#include "../src/Resources/LightBuffer.h"

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
	std::shared_ptr<Entity> get_entity_base(const std::string_view type, const size_t id);
	std::shared_ptr<Entity> new_entity(const std::string_view type, const size_t id, bool next_update = false);

	void load_resources();

	void update_entities();
	void render_entities();
	std::vector<std::shared_ptr<Entity>>* get_entities();
	std::shared_ptr<Entity> get_player();
	Grid<Entity>* get_entity_grid();

	void build_entity_grid();
	void add_to_grid(std::shared_ptr<Entity> entity);
	void remove_from_grid(std::shared_ptr<Entity> entity);

	LightBuffer& get_light_buffer();
private:
	std::map<size_t, GLuint> _programs;
	std::map<size_t, std::shared_ptr<Model>> _models;

	//std::map<std::string, std::map<size_t, std::shared_ptr<Entity>>> _entities;
	std::vector<std::shared_ptr<Entity>> _entities;
	std::vector<std::shared_ptr<Entity>> _entity_queue;

	// entity map to copy entites from instead of loading directly
	std::map<std::string, std::map<size_t, std::shared_ptr<Entity>>> _entities_base;

	std::shared_ptr<Entity> _player;

	Grid<Entity> _entity_grid;

	LightBuffer _lights;

	bool _load_programs();
	bool _load_program(const size_t id, const std::string_view path);

	bool _load_models();
	bool _load_model(const size_t model, const std::string_view path);

	bool _load_entities();

	bool _load_player();
};

#endif