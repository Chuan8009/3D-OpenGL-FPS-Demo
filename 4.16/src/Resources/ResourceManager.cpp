#include "ResourceManager.h"

#include <GL/gl3w.h>

#include <iostream>

#include "../src/Utility/FileReader.h"
#include "Program.h"
#include "Model.h"
#include "Entities/Entity.h"

#include "../src/System/Environment.h"
#include "../src/Resources/Window.h"
#include "../src/Resources/Camera.h"

#include "Entities/Components/ComponentLoader.h"
#include "Entities/Components/TransformComponent.h"

#define SHADER_FILE "Data\\Shaders\\shaders.txt"
#define SHADER_SECTION "Shaders"

#define FILE_SHADER_DIR "dir"
#define FILE_SHADER_VERTEX "vertex"
#define FILE_SHADER_FRAGMENT "fragment"

#define MODEL_FILE "Data\\Models\\models.txt"
#define MODEL_SECTION "Models"

#define ENTITY_FILE "Data\\Entities\\entities.txt"

ResourceManager::ResourceManager() :
	_entity_grid ( 10, 10, 500, 500 )
{}

ResourceManager::~ResourceManager() 
{}

GLuint ResourceManager::get_program(const size_t id) {
	return _programs[id];
}

std::shared_ptr<Model> ResourceManager::get_model(const size_t id) {
	return _models[id];
}

std::shared_ptr<Entity> ResourceManager::get_entity_base(const std::string_view type, const size_t id) {
	return _entities_base[type.data()][id];
}

std::shared_ptr<Entity> ResourceManager::new_entity(const std::string_view type, const size_t id, bool next_update) {
	const auto entity = _entities_base[type.data()][id];
	std::shared_ptr<Entity> new_entity = std::make_shared<Entity>(*entity);
	new_entity->copy(*entity);
	if (!next_update) {
		_entities.push_back(new_entity);
		add_to_grid(new_entity);
	}
	else {
		_entity_queue.push_back(new_entity);
	}
	return new_entity;
}

void ResourceManager::load_resources() {
	_load_programs();
	_load_models();
	_load_entities();
	_load_player();

	_lights._program = get_program(4);
}

bool ResourceManager::_load_programs() {
	FileReader file(SHADER_FILE, FileReader::int_val);

	if(!file.is_read()) {
		return false;
	}

	for (auto it = file.begin(); it != file.end(); ++it) {
		for (auto itt = it->table.begin(); itt != it->table.end(); ++itt) {
			std::cout << "Load Program: " << itt->value << '\n';
			_load_program(itt->key_val, itt->value);
		}
	}

	return true;
}

bool ResourceManager::_load_program(const size_t id, const std::string_view path) {
	if (_programs.count(id)) {
		return false;
	}

	FileReader file(path.data());

	Program program[] = { {GL_VERTEX_SHADER, }, { GL_FRAGMENT_SHADER, }, {GL_NONE, } };
	std::string_view dir, shader_path;

	auto append_strings = [](std::string& str, const std::string_view str2, const std::string_view str3) {
		str.reserve(str2.size() + str3.size());
		str.append(str2);
		str.append(str3);
	};

	file.read(&dir, FILE_SHADER_DIR);
	file.read(&shader_path, FILE_SHADER_VERTEX);
	append_strings(program[0].file_path, dir, shader_path);

	file.read(&shader_path, FILE_SHADER_FRAGMENT);
	append_strings(program[1].file_path, dir, shader_path);

	const GLuint program_id = load_shaders(program);

	_programs[id] = program_id;

	Environment::get().get_window()->get_camera()->attach_shader(program_id);

	return true;
}

bool ResourceManager::_load_models() {
	FileReader file(MODEL_FILE, FileReader::int_val);
	
	if(!file.is_read()) {
		return false;
	}

	for (auto it = file.begin(); it != file.end(); ++it) {
		for (auto itt = it->table.begin(); itt != it->table.end(); ++itt) {
			std::cout << "Load Model: " << itt->value << '\n';
			_load_model(itt->key_val, itt->value);
		}
	}

	return true;
}

bool ResourceManager::_load_model(const size_t id, const std::string_view path) {
	if (_models.count(id)) {
		return false;
	}

	auto model = std::make_shared<Model>(id, path);
	_models[id] = model;

	return true;
}

bool ResourceManager::_load_entities() {
	FileReader file(ENTITY_FILE, FileReader::int_val);

	if(!file.is_read()) {
		return false;
	}

	for (auto it = file.begin(); it != file.end(); ++it) {
		for (auto itt = it->table.begin(); itt != it->table.end(); ++itt) {
			std::cout << "Load Entity: " << itt->value << '\n';
			std::shared_ptr<Entity> entity = std::make_shared<Entity>(it->section, itt->key_val);
			load_components(entity);
			_entities_base[it->section][itt->key_val] = entity;
		}
	}

	return true;
}

bool ResourceManager::_load_player() {
	_player = new_entity(ENTITY_PLAYER, 0);
	return true;
}

void ResourceManager::update_entities() {
	_lights.clear();

	auto it = _entities.begin();
	while(it != _entities.end()) {
		if (!(*it)->get_destroy()) {
			(*it)->update();
			++it;
		}
		else {
			remove_from_grid(*it);
			it = _entities.erase(it);
		}
	}

	for(auto it = _entity_queue.begin(); it != _entity_queue.end(); ++it) {
		_entities.push_back(*it);
		add_to_grid(*it);
	}
	_entity_queue.clear();
}

void ResourceManager::render_entities() {
	_lights.update();

	for(auto e : _entities) {
		if (e->get_draw()) {
			if (auto transform = e->get<TransformComponent>()) {
				_models[e->get_model_id()]->draw(transform->_transform);
			}
		}
	}
}

void ResourceManager::build_entity_grid() {
	_entity_grid.clear();

	for(auto entity : _entities) {
		if(auto transform = entity->get<TransformComponent>()) {
			if (transform->_has_collision) {
				for (auto& mesh : _models[entity->get_model_id()]->_meshes) {
					auto bounding_box = mesh._bounding_box;
					bounding_box.min *= transform->_transform.get_scale();
					bounding_box.max *= transform->_transform.get_scale();
					bounding_box.min += transform->_transform.get_position();
					bounding_box.max += transform->_transform.get_position();

					_entity_grid.insert(bounding_box, entity);
				}
			}
		}
	}
}

void ResourceManager::add_to_grid(std::shared_ptr<Entity> entity) {
	if (auto transform = entity->get<TransformComponent>()) {
		if (transform->_has_collision) {
			for (auto& mesh : _models[entity->get_model_id()]->_meshes) {
				auto bounding_box = mesh._bounding_box;
				bounding_box.min *= transform->_transform.get_scale();
				bounding_box.max *= transform->_transform.get_scale();
				bounding_box.min += transform->_transform.get_position();
				bounding_box.max += transform->_transform.get_position();

				_entity_grid.insert(bounding_box, entity);
			}
		}
	}
}

void ResourceManager::remove_from_grid(std::shared_ptr<Entity> entity) {
	if (auto transform = entity->get<TransformComponent>()) {
		for (auto& mesh : _models[entity->get_model_id()]->_meshes) {
			auto bounding_box = mesh._bounding_box;
			bounding_box.min *= transform->_transform.get_scale();
			bounding_box.max *= transform->_transform.get_scale();
			bounding_box.min += transform->_transform.get_position();
			bounding_box.max += transform->_transform.get_position();

			_entity_grid.remove(bounding_box, entity);
		}
	}
}

std::vector<std::shared_ptr<Entity>>* ResourceManager::get_entities() {
	return &_entities;
}

std::shared_ptr<Entity> ResourceManager::get_player() {
	return _player;
}

Grid<Entity>* ResourceManager::get_entity_grid() {
	return &_entity_grid;
}

LightBuffer& ResourceManager::get_light_buffer() {
	return _lights;
}