#include "Model.h"

#include "ModelLoader.h"

Model::Model(const GLuint program, const std::string directory, const std::string model_file) :
	_program ( program )
{
	load_assimp(directory, model_file, meshes);
}

Model::Model(const GLuint program, const char* file_path) :
	_program ( program )
{
	load_model_file(file_path, meshes);
}

void Model::draw() const {
	for(auto mesh : meshes) {
		mesh.draw(_program);
	}
}