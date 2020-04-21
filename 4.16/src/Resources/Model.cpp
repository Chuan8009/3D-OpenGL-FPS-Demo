#include "Model.h"

#include "ModelLoader.h"

#include "../src/System/Environment.h"
#include "ResourceManager.h"

Model::Model(const GLuint program, const std::string_view directory, const std::string_view model_file) :
	_program ( program )
{
	load_assimp(directory, model_file, meshes);
}

Model::Model(const size_t id, std::string_view file_path) :
	_id		( id )
{
	load_model_file(file_path.data(), _program, meshes);
}

void Model::draw() const {
	for(auto mesh : meshes) {
		mesh.draw(_program);
	}
}