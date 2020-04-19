#include "ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL/SOIL2.h>

#include <iostream>

#include "FileReader.h"
#include "Program.h"

#include "Environment.h"
#include "ResourceManager.h"

#define FILE_MODEL_SECTION "Model"
#define FILE_MODEL_DIR "dir"
#define FILE_MODEL_FILE "file"
#define FILE_MODEL_PROGRAM_ID "program_id"

void load_material_textures(std::vector<Texture>& textures, aiMaterial* material, const aiTextureType type, const std::string type_name, const std::string directory) {
	for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
		aiString string;
		material->GetTexture(type, i, &string);
		Texture texture;
		std::string path(string.C_Str());
		size_t end = path.find_last_of('\\') + 1;
		path.erase(0, end);
		std::cout << "Texture Path: " << directory + path << '\n';
		texture.id = SOIL_load_OGL_texture((directory + path).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
		if (texture.id == 0) {
			std::cout << SOIL_last_result() << '\n';
		}

		std::cout << "id: " << texture.id << '\n';
		texture.type = type_name;
		textures.push_back(texture);
	}
}

bool load_assimp(const std::string directory, std::string path, std::vector<Mesh>& meshes) {
	Assimp::Importer importer;
	path = directory + path;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene) {
		std::cout << "Assimp Loader -- Couldn't load scene at -- " << path << '\n';
		std::cout << importer.GetErrorString();
		return false;
	}

	const aiMesh* ai_mesh = scene->mMeshes[0];
	std::cout << "Meshes: " << scene->mNumMeshes << '\n';
	std::cout << "Materials: " << scene->mNumMaterials << '\n';
	std::cout << "Textures: " << scene->mNumTextures << '\n';

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		Mesh mesh;
		ai_mesh = scene->mMeshes[i];
		mesh.vertices.reserve(ai_mesh->mNumVertices);
		mesh.uvs.reserve(ai_mesh->mNumVertices);
		mesh.normals.reserve(ai_mesh->mNumVertices);
		const aiVector3D no_texture_coord(0.0f, 0.0f, 0.0f);
		for (unsigned int i = 0; i < ai_mesh->mNumVertices; ++i) {
			aiVector3D pos = ai_mesh->mVertices[i];
			mesh.vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
			aiVector3D uv = ai_mesh->HasTextureCoords(0) ? ai_mesh->mTextureCoords[0][i] : no_texture_coord;
			mesh.uvs.push_back(glm::vec2(uv.x, uv.y));
			aiVector3D normal = ai_mesh->mNormals[i];
			mesh.normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
		}

		mesh.indices.reserve(ai_mesh->mNumFaces);
		for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) {
			mesh.indices.push_back(ai_mesh->mFaces[i].mIndices[0]);
			mesh.indices.push_back(ai_mesh->mFaces[i].mIndices[1]);
			mesh.indices.push_back(ai_mesh->mFaces[i].mIndices[2]);
		}

		mesh.load_buffers();

		if (ai_mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];

			std::cout << "Texture Count: " << material->GetTextureCount(aiTextureType_DIFFUSE) << '\n';
			load_material_textures(mesh.textures, material, aiTextureType_DIFFUSE, "diffuse", directory);
			load_material_textures(mesh.textures, material, aiTextureType_SPECULAR, "specular", directory);

		}

		meshes.push_back(mesh);
	}

	return true;
}

bool load_model_file(const char* file_path, GLuint& program, std::vector<Mesh>& meshes) {
	FileReader file(file_path);
	bool loaded = false;

	std::string model_file;
	std::string directory;
	size_t program_id;

	file.set_section(FILE_MODEL_SECTION);
	file.read(&model_file, FILE_MODEL_FILE);
	file.read(&directory, FILE_MODEL_DIR);
	file.read(&program_id, FILE_MODEL_PROGRAM_ID);

	loaded = load_assimp(directory, model_file, meshes);

	program = Environment::get().get_resource_manager()->get_program(program_id);
	if(program == 0) {
		std::cout << "Warning: Shader Error -- index: " << program_id << " doesn't exist" << '\n';
	}

	return loaded;
}