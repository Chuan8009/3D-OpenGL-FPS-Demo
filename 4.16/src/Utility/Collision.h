#ifndef COLLISION_H
#define COLLISION_H

#include <glm/gtc/matrix_transform.hpp>

struct Bounding_Box {
	glm::vec3 min, max;
};

inline bool collision(Bounding_Box a, Bounding_Box b) {
	/*
	return a.min.x <= b.max.x && a.max.x >= b.min.x &&
		   a.min.y <= b.max.y && a.max.y >= b.min.y &&
		   a.min.z <= b.max.z && a.max.z >= b.min.z;
		   */

	return !(a.max.x <= b.min.x || a.min.x >= b.max.x ||
		     a.max.y <= b.min.y || a.min.y >= b.max.y ||
		     a.max.z <= b.min.z || a.min.z >= b.max.z);
}

#endif