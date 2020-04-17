#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

class Model {
public:
	Model(GLuint program, std::string directory, std::string model_file);
	Model(const char* file);

	void draw();

	std::vector<Mesh> meshes;
private:
	GLuint _program;
};

#endif