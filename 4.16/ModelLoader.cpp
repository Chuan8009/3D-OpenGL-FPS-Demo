#include "ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL/SOIL2.h>

#include <iostream>

void load_material_textures(std::vector<Texture>& textures, aiMaterial* material, aiTextureType type, std::string type_name, std::string directory) {
	for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
		aiString string;
		material->GetTexture(type, i, &string);
		Texture texture;
		std::string path(string.C_Str());
		size_t end = path.find_last_of('\\') + 1;
		path.erase(0, end);
		std::cout << "Texture Path: " << directory + path << std::endl;
		texture.id = SOIL_load_OGL_texture((directory + path).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
		if (texture.id == 0) {
			std::cout << SOIL_last_result() << std::endl;
		}

		std::cout << "id: " << texture.id << std::endl;
		texture.type = type_name;
		textures.push_back(texture);
	}
}

bool load_assimp(std::string directory, std::string path, std::vector<Mesh>& meshes) {
	Assimp::Importer importer;
	path = directory + path;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene) {
		std::cout << "Assimp Loader -- Couldn't load scene at -- " << path << std::endl;
		std::cout << importer.GetErrorString();
		return false;
	}

	const aiMesh* ai_mesh = scene->mMeshes[0];
	std::cout << "Meshes: " << scene->mNumMeshes << std::endl;
	std::cout << "Materials: " << scene->mNumMaterials << std::endl;
	std::cout << "Textures: " << scene->mNumTextures << std::endl;

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

			std::cout << "Texture Count: " << material->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
			load_material_textures(mesh.textures, material, aiTextureType_DIFFUSE, "diffuse", directory);
			load_material_textures(mesh.textures, material, aiTextureType_SPECULAR, "specular", directory);

		}

		meshes.push_back(mesh);
	}

	return true;
}