#ifndef COMPONENT_LOADER_H
#define COMPONENT_LOADER_H

#include <memory>

class Entity;

bool load_components(std::shared_ptr<Entity> entity);

#endif