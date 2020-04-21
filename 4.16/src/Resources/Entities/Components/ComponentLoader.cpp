#include "ComponentLoader.h"

#include <iostream>

#include "../src/Resources/Entities/Entity.h"
#include "../src/Resources/Entities/Components/TransformComponent.h"

#include "../src/Utility/FileReader.h"

#define ENTITIES_FILE "Data\\Entities\\entities.txt"

#define ENTITY_SECTION "Entity"
#define ENTITY_NAME "str_name"

#define TRANSFORM_SECTION "Transform"

void load_transform(const FileReader& file, std::shared_ptr<Entity> entity, std::shared_ptr<TransformComponent> &transform) {
	// load stuff

	transform = std::make_shared<TransformComponent>(entity);
}


bool load_components(std::shared_ptr<Entity> entity) {
	FileReader file(ENTITIES_FILE, FileReader::int_val);

	file.set_section(entity->get_type());
	
	std::string path;
	if(!file.read(&path, entity->get_id())) {
		std::cout << "Couldn't load entity with id -- " << entity->get_id() << '\n';
		return false;
	}

	entity->clear();
	FileReader entity_file(path.c_str());

	std::string name;
	if (entity_file.set_section(ENTITY_SECTION)) {
		entity_file.read(&name, ENTITY_NAME);


		entity->set_name(name);
	}

	if(entity_file.set_section(TRANSFORM_SECTION)) {
		std::shared_ptr<TransformComponent> transform = nullptr;
		load_transform(entity_file, entity, transform);
		entity->add(transform);
	}

	return true;
}