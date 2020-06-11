#ifndef MESH_H
#define MESH_H

#include <GL/gl3w.h>

#include <vector>

#include "Transform.h"
#include "Texture.h"
#include "../src/Utility/Collision.h"

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
	void draw(const GLuint program, Transform& transform);
	void draw_lines(const GLuint program, Transform& transform);
	void make_bounding_box();
public:
	std::vector<Texture>        _textures;
	std::vector<glm::vec3>      _vertices;
	std::vector<glm::vec2>      _uvs;
	std::vector<glm::vec3>      _normals;
	std::vector<unsigned short> _indices;

	Transform _transform;

	Bounding_Box _bounding_box;
private:
	GLuint  _vao,
	     	_vertex_buffer,
		    _uv_buffer,
		    _normal_buffer,
		    _indices_buffer;
};

#endif