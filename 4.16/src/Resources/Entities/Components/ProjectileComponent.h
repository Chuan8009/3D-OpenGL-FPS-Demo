#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include "Component.h"

#include "../src/Utility/Timer.h"

#include <glm/gtc/matrix_transform.hpp>

class Entity;

class ProjectileComponent : public Component {
public:
	ProjectileComponent(std::shared_ptr<Entity> entity, const double duration);
	ProjectileComponent(std::shared_ptr<Entity> new_entity, const ProjectileComponent& rhs);
	std::shared_ptr<Component> copy(std::shared_ptr<Entity> new_entity) const;

	void update();

	const int get_type() const;

	static constexpr int _type = PROJECTILE_COMPONENT;

	void fire(glm::vec3 dir);

	bool _moving;
	glm::vec3 _dir;

	double _duration;
	Timer _timer;
};

#endif