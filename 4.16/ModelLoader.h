#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "Mesh.h"

#include <string>
#include <vector>

bool load_assimp(const std::string directory, std::string path, std::vector<Mesh>& meshes);

bool load_model_file(const char* file_path, std::vector<Mesh>& meshes);

#endif