#ifndef MESH_H
#define MESH_H

#include <GL/gl3w.h>

#include <vector>

#include "Transform.h"
#include "Texture.h"

class Mesh {
public:
	Mesh();
	Mesh(
		const std::vector<Texture>& textures,
		const std::vector<glm::vec3>& vertices,
		const std::vector<glm::vec2>& uvs,
		const std::vector<glm::vec3>& normals,
		const std::vector<unsigned short>& indices,
		const Transform& transform
	);

	void load_buffers();
	void draw(const GLuint program);
public:
	std::vector<Texture>        textures;
	std::vector<glm::vec3>      vertices;
	std::vector<glm::vec2>      uvs;
	std::vector<glm::vec3>      normals;
	std::vector<unsigned short> indices;

	Transform transform;
private:
	GLuint  _vao,
	     	_vertex_buffer,
		    _uv_buffer,
		    _normal_buffer,
		    _indices_buffer;
};

#endif