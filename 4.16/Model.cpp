#include "Model.h"

#include "ModelLoader.h"

#include "Environment.h"
#include "ResourceManager.h"

Model::Model(const GLuint program, const std::string directory, const std::string model_file) :
	_program ( program )
{
	load_assimp(directory, model_file, meshes);
}

Model::Model(const char* file_path)
{
	load_model_file(file_path, _program, meshes);
}

void Model::draw() const {
	for(auto mesh : meshes) {
		mesh.draw(_program);
	}
}