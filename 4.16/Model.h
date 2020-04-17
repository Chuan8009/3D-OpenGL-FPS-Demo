#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

class Model {
public:
	Model(const GLuint program, const std::string directory, const std::string model_file);
	Model(const GLuint program, const char *file_path);

	void draw() const;

	std::vector<Mesh> meshes;
private:
	GLuint _program;
};

#endif