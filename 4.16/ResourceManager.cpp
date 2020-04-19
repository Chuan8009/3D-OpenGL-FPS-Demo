#include "ResourceManager.h"

#include <GL/gl3w.h>

#include <iostream>

#include "FileReader.h"
#include "Program.h"

#include "Environment.h"
#include "Window.h"
#include "Camera.h"

#define SHADER_FILE "Data\\Shaders\\shaders.txt"
#define SHADER_SECTION "Shaders"

#define FILE_SHADER_DIR "dir"
#define FILE_SHADER_VERTEX "vertex"
#define FILE_SHADER_FRAGMENT "fragment"

ResourceManager::ResourceManager()
{
	_load_programs();
}

ResourceManager::~ResourceManager() {

}

bool ResourceManager::load_model(const size_t id) {
	return 0;
}

GLuint ResourceManager::get_program(const size_t id) {
	return _programs[id];
}

std::shared_ptr<Model> ResourceManager::get_model(const size_t id) {
	return nullptr;
}

bool ResourceManager::_load_programs() {
	FileReader file(SHADER_FILE, FileReader::int_val);

	for(auto it = file.begin(); it != file.end(); ++it) {
		std::cout << "Load Program: " << it->value << '\n';
		_load_program(it->key_val, it->value);
	}

	return false;
}

bool ResourceManager::_load_program(const size_t id, const std::string path) {
	if (_programs.find(id) != _programs.end()) {
		return false;
	}

	FileReader file(path.c_str());

	Program program[] = { {GL_VERTEX_SHADER, }, { GL_FRAGMENT_SHADER, }, {GL_NONE, } };
	std::string dir, shader_path;

	file.read(&dir, FILE_SHADER_DIR);
	file.read(&shader_path, FILE_SHADER_VERTEX);
	program[0].file_path = dir + shader_path;
	file.read(&shader_path, FILE_SHADER_FRAGMENT);
	program[1].file_path = dir + shader_path;

	const GLuint program_id = load_shaders(program);

	_programs[id] = program_id;

	Environment::get().get_window()->get_camera()->attach_shader(program_id);

	return true;
}