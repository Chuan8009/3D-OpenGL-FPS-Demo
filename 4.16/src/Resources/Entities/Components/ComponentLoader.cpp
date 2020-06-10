#include "ComponentLoader.h"

#include <iostream>
#include <sstream>

#include "../src/Resources/Entities/Entity.h"
#include "../src/Resources/Entities/Components/PlayerComponent.h"
#include "../src/Resources/Entities/Components/TransformComponent.h"
#include "../src/Resources/Entities/Components/ProjectileComponent.h"

#include "../src/Utility/FileReader.h"

#include "../src/System/Environment.h"
#include "../src/Resources/ResourceManager.h"
#include "../src/Resources/Model.h"

#define ENTITIES_FILE "Data\\Entities\\entities.txt"

#define PLAYER_SECTION "Player"
#define PLAYER_FIRE_RATE "fire_rate"
#define PLAYER_WEIGHT "weight"

#define ENTITY_SECTION "Entity"
#define ENTITY_MODEL_ID "model_id"
#define ENTITY_NAME "str_name"

#define TRANSFORM_SECTION "Transform"
#define TRANSFORM_SCALE "scale"
#define TRANSFORM_ROTATION "rotation"
#define TRANSFORM_SPEED "speed"

#define PROJECTILE_SECTION "Projectile"
#define PROJECTILE_DURATION "duration"

void load_player(FileReader& file, std::shared_ptr<Entity> entity, std::shared_ptr<PlayerComponent>& player) {
	float fire_rate = 1.0f;
	float weight = 0.01f;

	file.read(&fire_rate, PLAYER_FIRE_RATE);
	file.read(&weight, PLAYER_WEIGHT);

	player = std::make_shared<PlayerComponent>(entity, fire_rate, weight);
}

void load_transform(FileReader& file, std::shared_ptr<Entity> entity, std::shared_ptr<TransformComponent> &transform) {
	size_t model_id = 0;
	glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
	float speed = 0.1f;
	
	file.read(&speed, TRANSFORM_SPEED);

	std::string data;
	if (file.read(&data, TRANSFORM_SCALE)) {
		std::stringstream scale_stream(data);
		scale_stream >> scale.x >> scale.y >> scale.z;
	}

	if (file.read(&data, TRANSFORM_ROTATION)) {
		std::stringstream rotation_stream(data);
		rotation_stream >> rotation.x >> rotation.y >> rotation.z;
	}

	transform = std::make_shared<TransformComponent>(entity, scale, rotation, speed);
}

void load_projectile(FileReader& file, std::shared_ptr<Entity> entity, std::shared_ptr<ProjectileComponent> &projetile) {
	double duration = 0;

	file.read(&duration, PROJECTILE_DURATION);

	projetile = std::make_shared<ProjectileComponent>(entity, duration);
}

bool load_components(std::shared_ptr<Entity> entity) {
	FileReader file(ENTITIES_FILE, FileReader::int_val);

	file.set_section(entity->get_type());
	
	std::string_view path;
	if(!file.read(&path, entity->get_id())) {
		std::cout << "Couldn't load entity with id -- " << entity->get_id() << '\n';
		return false;
	}

	entity->clear();
	FileReader entity_file(path.data());

	int model_id = 0;
	std::string_view name;
	if (entity_file.set_section(ENTITY_SECTION)) {
		entity_file.read(&model_id, ENTITY_MODEL_ID);
		entity_file.read(&name, ENTITY_NAME);

		entity->set_model_id(model_id);
		entity->set_name(name);
	}

	if(entity_file.set_section(PLAYER_SECTION)) {
		std::shared_ptr<PlayerComponent> player = nullptr;
		load_player(entity_file, entity, player);
		entity->add(player);
	}

	if(entity_file.set_section(TRANSFORM_SECTION)) {
		std::shared_ptr<TransformComponent> transform = nullptr;
		load_transform(entity_file, entity, transform);
		entity->add(transform);
	}

	if(entity_file.set_section(PROJECTILE_SECTION)) {
		std::shared_ptr<ProjectileComponent> projectile = nullptr;
		load_projectile(entity_file, entity, projectile);
		entity->add(projectile);
	}

	return true;
}