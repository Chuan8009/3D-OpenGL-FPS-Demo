#include "LightComponent.h"

#include "../src/System/Environment.h"
#include "../src/Resources/ResourceManager.h"

#include "../src/Resources/Entities/Entity.h"
#include "../src/Resources/Entities/Components/TransformComponent.h"

LightComponent::LightComponent(std::shared_ptr<Entity> entity, int program_id, glm::vec3 color, float intensity, bool static_position, glm::vec3 offset) :
	Component		 ( entity ),
	_program		 ( Environment::get().get_resources()->get_program(program_id) ),
	_color			 ( color ),
	_intensity	     ( intensity ),
	_offset			 ( offset ),
	_static_position ( static_position )
{}

LightComponent::LightComponent(std::shared_ptr<Entity> new_entity, const LightComponent& rhs) :
	Component		 ( new_entity ),
	_program		 ( rhs._program ),
	_color			 ( rhs._color ),
	_intensity		 ( rhs._intensity ),
	_offset			 ( rhs._offset ),
	_static_position ( rhs._static_position )
{}

std::shared_ptr<Component> LightComponent::copy(std::shared_ptr<Entity> new_entity) const {
	return std::static_pointer_cast<Component>(std::make_shared<LightComponent>(new_entity, *this));
}

void LightComponent::update() {
	if(!_static_position) {
		Environment::get().get_resources()->get_light_buffer().add(shared_from_this());
	}
}

void LightComponent::on_collision(std::shared_ptr<Entity> entity) {

}

const int LightComponent::get_type() const {
	return LIGHT_COMPONENT;
}