#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <array>
#include <map>

#include <memory>

struct Program;
class Model;

typedef unsigned int GLuint;

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	bool load_model(const size_t model);

	GLuint get_program(const size_t id);
	std::shared_ptr<Model> get_model(const size_t id);
private:
	std::map<size_t, GLuint> _programs;
	std::map<size_t, std::shared_ptr<Model>> _models;

	bool _load_programs();
	bool _load_program(const size_t id, const std::string path);
};

#endif