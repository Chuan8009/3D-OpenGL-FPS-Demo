#include "Model.h"

#include "ModelLoader.h"

Model::Model(GLuint program, std::string directory, std::string model_file) :
	_program ( program )
{
	load_assimp(directory, model_file, meshes);
}

Model::Model(const char* file) {

}

void Model::draw() {
	for(auto mesh : meshes) {
		mesh.draw(_program);
	}
}